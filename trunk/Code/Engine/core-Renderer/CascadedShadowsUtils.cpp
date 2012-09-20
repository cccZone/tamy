#include "core-Renderer/CascadedShadowsUtils.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/Viewport.h"
#include "core/Math.h"


///////////////////////////////////////////////////////////////////////////////

void CascadedShadowsUtils::calculateCascadesBounds( const CascadesConfig& config, CascadeStage* outArrCascadeStages )
{
   // calculate the camera depth range
   Camera* activeCamera = config.m_activeCamera;
   float nearCameraClippingPlane = activeCamera->getNearClippingPlane();
   float farCameraClippingPlane = activeCamera->getFarClippingPlane();

   // pre-calculate light camera transformations that we'll use later on during ;AABBs calculations
   Matrix lightViewMtx, invLightViewMtx;
   MatrixUtils::calculateViewMtx( config.m_lightRotationMtx, lightViewMtx );
   invLightViewMtx.setInverse( lightViewMtx );

   // acquire scene's AABB and transform it to the light view space
   Vector sceneBBPointsLightSpace[8];
   {
      Vector sceneAABBPoints[8];
      createBBPoints( config.m_sceneBounds, sceneAABBPoints );

      // Transform the scene bounding box to light space.
      for( int i = 0; i < 8; ++i ) 
      {
         lightViewMtx.transform( sceneAABBPoints[i], sceneBBPointsLightSpace[i] );
      }
   }

   // calculate light frustum bounds
   Vector worldUnitsPerTexel;
   uint numCascades = config.m_numCascades;
   uint cascadesInRow = config.m_numCascadesPerTextureRow;
   AABoundingBox cascadeFrustumBoundsWS;

   // the furthest light distance from origin is a negative number equal to the lowest min.z value
   // of the frustum bounds and defines the distance at which we should move the light away
   // from the camera position along its look axis to get all the scene objects in the view
   float furthestCameraDistanceFromOrigin = 0.0f;

   for ( uint i = 0; i < numCascades; ++i )
   {
      float frustumIntervalBegin = max2( nearCameraClippingPlane, config.m_cascadeIntervals[i] );
      float frustumIntervalEnd = min2( farCameraClippingPlane, config.m_cascadeIntervals[i + 1] );
      outArrCascadeStages[i].m_cameraNearZ = frustumIntervalBegin;
      outArrCascadeStages[i].m_cameraFarZ = frustumIntervalEnd - nearCameraClippingPlane; // counter the near camera clipping plane

      calculateCameraCascadeFrustumBounds( activeCamera, frustumIntervalBegin, frustumIntervalEnd, cascadeFrustumBoundsWS );

      // transform the bounds to light
      AABoundingBox& lightFrustumBounds = outArrCascadeStages[i].m_lightFrustumBounds;
      cascadeFrustumBoundsWS.transform( lightViewMtx, lightFrustumBounds );

      // now adjust the light frustum a bit in order to remove the shimmering effect
      // of shadow edges whenever the camera is moved
      float cascadeDimensions = config.m_cascadeDimensions;

      {
         // We calculate a looser bound based on the size of the PCF blur.  This ensures us that we're 
         // sampling within the correct map.
         float scaleDueToBlurAMT = ( (float)( config.m_pcfBlurSize * 2 + 1 ) / cascadeDimensions );
         Vector vScaleDueToBlurAMT( scaleDueToBlurAMT, scaleDueToBlurAMT, 0.0f, 0.0f );

         float normalizeByBufferSize = ( 1.0f / cascadeDimensions );
         Vector vNormalizeByBufferSize( normalizeByBufferSize, normalizeByBufferSize, 0.0f, 0.0f );

         // We calculate the offsets as a percentage of the bound.
         Vector broaderOffset;
         broaderOffset.setSub( lightFrustumBounds.max, lightFrustumBounds.min ).mul( 0.5f ).mul( vScaleDueToBlurAMT );

         lightFrustumBounds.max.add( broaderOffset );
         lightFrustumBounds.min.sub( broaderOffset );

         // The world units per texel are used to snap the orthographic projection to texel sized increments.  
         // Because we're fitting tightly to the cascades, the shimmering shadow edges will still be present when the 
         // camera rotates.  However, when zooming in or strafing the shadow edge will not shimmer.
         worldUnitsPerTexel.setSub( lightFrustumBounds.max, lightFrustumBounds.min ).mul( vNormalizeByBufferSize );
      }

      // Snap the camera to 1-pixel increments so that camera movement doesn't cause the shadows to jitter around the edges
      {
         lightFrustumBounds.min.div( worldUnitsPerTexel );
         lightFrustumBounds.min.floor();
         lightFrustumBounds.min.mul( worldUnitsPerTexel );

         lightFrustumBounds.max.div( worldUnitsPerTexel );
         lightFrustumBounds.max.floor();
         lightFrustumBounds.max.mul( worldUnitsPerTexel );
      }

      calculateLightClippingPlanes( sceneBBPointsLightSpace, lightFrustumBounds );

      // find the furthest distance of the camera from the origin for all calculated cascades
      if ( lightFrustumBounds.min.z < furthestCameraDistanceFromOrigin )
      {
         furthestCameraDistanceFromOrigin = lightFrustumBounds.min.z;
      }

      // calculate viewport
      {
         Viewport& viewport = outArrCascadeStages[i].m_viewport;
         viewport.m_minZ = 0.0f;
         viewport.m_maxZ = 1.0f;
         viewport.m_offsetX = (ulong)( ( i % cascadesInRow ) * cascadeDimensions );
         viewport.m_offsetY = (ulong)( ( i / cascadesInRow ) * cascadeDimensions );
         viewport.m_width = (ulong)cascadeDimensions;
         viewport.m_height = (ulong)cascadeDimensions;
      }

      // calculate objects query bounds ( in world space )
      lightFrustumBounds.transform( invLightViewMtx, outArrCascadeStages[i].m_objectsQueryBounds );
   }


   // adjust frustum clipping planes keeping in mind that the camera's gonna be offset
   // and calculate the light position
   for ( uint i = 0; i < numCascades; ++i )
   {
      AABoundingBox& lightFrustumBounds = outArrCascadeStages[i].m_lightFrustumBounds;

      lightFrustumBounds.min.z -= furthestCameraDistanceFromOrigin;
      lightFrustumBounds.max.z -= furthestCameraDistanceFromOrigin;

      Vector frustumCenter;
      frustumCenter.setAdd( lightFrustumBounds.min, lightFrustumBounds.max ).mul( 0.5f );
      frustumCenter.z = furthestCameraDistanceFromOrigin;

      Vector lightPos;
      invLightViewMtx.transform( frustumCenter, lightPos );

      Matrix& lightMtx = outArrCascadeStages[i].m_lightMtx;
      lightMtx = config.m_lightRotationMtx;
      lightMtx.setPosition( lightPos );
   }
}

///////////////////////////////////////////////////////////////////////////////

void CascadedShadowsUtils::calculateCameraCascadeFrustumBounds( Camera* activeCamera, float intervalBegin, float intervalEnd, AABoundingBox& outFrustumPart )
{
   float prevNearZ, prevFarZ;
   activeCamera->getClippingPlanes( prevNearZ, prevFarZ );
   activeCamera->setClippingPlanes( intervalBegin, intervalEnd );

   Frustum frustum;
   activeCamera->calculateFrustum( frustum );
   frustum.calculateBoundingBox( outFrustumPart );

   activeCamera->setClippingPlanes( prevNearZ, prevFarZ );
}

///////////////////////////////////////////////////////////////////////////////

void CascadedShadowsUtils::createBBPoints( const AABoundingBox& inAABB, Vector* outBBPoints )
{
   //This map enables us to use a for loop and do vector math.
   static const Vector extentsMap[] = 
   { 
      Vector( 1.0f, 1.0f, -1.0f, 1.0f ), 
      Vector( -1.0f, 1.0f, -1.0f, 1.0f ), 
      Vector( 1.0f, -1.0f, -1.0f, 1.0f ),
      Vector( -1.0f, -1.0f, -1.0f, 1.0f ),
      Vector( 1.0f, 1.0f, 1.0f, 1.0f ),
      Vector( -1.0f, 1.0f, 1.0f, 1.0f ),
      Vector( 1.0f, -1.0f, 1.0f, 1.0f ),
      Vector( -1.0f, -1.0f, 1.0f, 1.0f )
   };

   Vector extents;
   extents.setSub( inAABB.max, inAABB.min ).mul( 0.5f );

   Vector center;
   center.setAdd( inAABB.min, inAABB.max ).mul( 0.5f );

   for( int i = 0; i < 8; ++i ) 
   {
      outBBPoints[i].setMulAdd( extents, extentsMap[i], center );
   }
}

///////////////////////////////////////////////////////////////////////////////

void CascadedShadowsUtils::calculateLightClippingPlanes( const Vector* sceneBBInLightSpace, AABoundingBox& inOutLightFrustumBounds )
{
   struct Triangle 
   {
      Vector   pt[3];
      bool     culled;
   };

   // Initialize the near and far planes
   float nearPlane = FLT_MAX;
   float farPlane = -FLT_MAX;

   Triangle triangleList[16];
   int triangleCount = 1;

   triangleList[0].pt[0] = sceneBBInLightSpace[0];
   triangleList[0].pt[1] = sceneBBInLightSpace[1];
   triangleList[0].pt[2] = sceneBBInLightSpace[2];
   triangleList[0].culled = false;

   // These are the indices used to tesselate an AABB into a list of triangles.
   static const int bbTriIndexes[] = 
   {
      0,1,2,  1,2,3,
      4,5,6,  5,6,7,
      0,2,4,  2,4,6,
      1,3,5,  3,5,7,
      0,1,4,  1,4,5,
      2,3,6,  3,6,7 
   };

   int pointPassesCollision[3];

   // At a high level: 
   // 1. Iterate over all 12 triangles of the AABB.  
   // 2. Clip the triangles against each plane. Create new triangles as needed.
   // 3. Find the min and max z values as the near and far plane.

   //This is easier because the triangles are in camera spacing making the collisions tests simple comparisions.

   float lightCameraOrthographicMinX = inOutLightFrustumBounds.min.x;
   float lightCameraOrthographicMaxX = inOutLightFrustumBounds.max.x;
   float lightCameraOrthographicMinY = inOutLightFrustumBounds.min.y;
   float lightCameraOrthographicMaxY = inOutLightFrustumBounds.max.y;

   for( int bbTriIter = 0; bbTriIter < 12; ++bbTriIter ) 
   {

      triangleList[0].pt[0] = sceneBBInLightSpace[ bbTriIndexes[ bbTriIter*3 + 0 ] ];
      triangleList[0].pt[1] = sceneBBInLightSpace[ bbTriIndexes[ bbTriIter*3 + 1 ] ];
      triangleList[0].pt[2] = sceneBBInLightSpace[ bbTriIndexes[ bbTriIter*3 + 2 ] ];
      triangleCount = 1;
      triangleList[0].culled = false;

      // Clip each individual triangle against the 4 frustums.  When ever a triangle is clipped into new triangles, 
      //add them to the list.
      for( int frustumPlaneIter = 0; frustumPlaneIter < 4; ++frustumPlaneIter ) 
      {

         float edge;
         int component;

         if( frustumPlaneIter == 0 ) 
         {
            edge = lightCameraOrthographicMinX;
            component = 0;
         } 
         else if( frustumPlaneIter == 1 ) 
         {
            edge = lightCameraOrthographicMaxX;
            component = 0;
         } 
         else if( frustumPlaneIter == 2 ) 
         {
            edge = lightCameraOrthographicMinY;
            component = 1;
         } 
         else 
         {
            edge = lightCameraOrthographicMaxY;
            component = 1;
         }

         for( int triIter=0; triIter < triangleCount; ++triIter ) 
         {
            // We don't delete triangles, so we skip those that have been culled.
            if( !triangleList[triIter].culled ) 
            {
               int insideVertCount = 0;
               Vector tempOrder;

               // Test against the correct frustum plane.
               // This could be written more compactly, but it would be harder to understand.

               if( frustumPlaneIter == 0 ) 
               {
                  for( int triPtIter = 0; triPtIter < 3; ++triPtIter ) 
                  {
                     if( triangleList[triIter].pt[triPtIter].x > lightCameraOrthographicMinX ) 
                     { 
                        pointPassesCollision[triPtIter] = 1;
                     }
                     else 
                     {
                        pointPassesCollision[triPtIter] = 0;
                     }
                     insideVertCount += pointPassesCollision[triPtIter];
                  }
               }
               else if( frustumPlaneIter == 1 ) 
               {
                  for( int triPtIter = 0; triPtIter < 3; ++triPtIter ) 
                  {
                     if( triangleList[triIter].pt[triPtIter].x < lightCameraOrthographicMaxX )
                     {
                        pointPassesCollision[triPtIter] = 1;
                     }
                     else
                     { 
                        pointPassesCollision[triPtIter] = 0;
                     }
                     insideVertCount += pointPassesCollision[triPtIter];
                  }
               }
               else if( frustumPlaneIter == 2 ) 
               {
                  for( int triPtIter = 0; triPtIter < 3; ++triPtIter ) 
                  {
                     if( triangleList[triIter].pt[triPtIter].y > lightCameraOrthographicMinY ) 
                     {
                        pointPassesCollision[triPtIter] = 1;
                     }
                     else 
                     {
                        pointPassesCollision[triPtIter] = 0;
                     }
                     insideVertCount += pointPassesCollision[triPtIter];
                  }
               }
               else 
               {
                  for( int triPtIter = 0; triPtIter < 3; ++triPtIter ) 
                  {
                     if( triangleList[triIter].pt[triPtIter].y < lightCameraOrthographicMaxY ) 
                     {
                        pointPassesCollision[triPtIter] = 1;
                     }
                     else 
                     {
                        pointPassesCollision[triPtIter] = 0;
                     }
                     insideVertCount += pointPassesCollision[triPtIter];
                  }
               }

               // Move the points that pass the frustum test to the beginning of the array.
               if( pointPassesCollision[1] && !pointPassesCollision[0] ) 
               {
                  tempOrder =  triangleList[triIter].pt[0];   
                  triangleList[triIter].pt[0] = triangleList[triIter].pt[1];
                  triangleList[triIter].pt[1] = tempOrder;
                  pointPassesCollision[0] = true;            
                  pointPassesCollision[1] = false;            
               }
               if( pointPassesCollision[2] && !pointPassesCollision[1] ) 
               {
                  tempOrder =  triangleList[triIter].pt[1];   
                  triangleList[triIter].pt[1] = triangleList[triIter].pt[2];
                  triangleList[triIter].pt[2] = tempOrder;
                  pointPassesCollision[1] = true;            
                  pointPassesCollision[2] = false;                        
               }
               if( pointPassesCollision[1] && !pointPassesCollision[0] ) 
               {
                  tempOrder =  triangleList[triIter].pt[0];   
                  triangleList[triIter].pt[0] = triangleList[triIter].pt[1];
                  triangleList[triIter].pt[1] = tempOrder;
                  pointPassesCollision[0] = true;            
                  pointPassesCollision[1] = false;            
               }

               if( insideVertCount == 0 ) 
               { // All points failed. We're done,  
                  triangleList[triIter].culled = true;
               }
               else if( insideVertCount == 1 ) 
               {// One point passed. Clip the triangle against the Frustum plane
                  triangleList[triIter].culled = false;

                  Vector vert0ToVert1, vert0ToVert2;
                  vert0ToVert1.setSub( triangleList[triIter].pt[1], triangleList[triIter].pt[0] );
                  vert0ToVert2.setSub( triangleList[triIter].pt[2], triangleList[triIter].pt[0] );

                  // Find the collision ratio.
                  float hitPointTimeRatio = edge - triangleList[triIter].pt[0][component];

                  // Calculate the distance along the vector as ratio of the hit ratio to the component.
                  float distanceAlongVector01 = hitPointTimeRatio / vert0ToVert1[component];
                  float distanceAlongVector02 = hitPointTimeRatio / vert0ToVert2[component];

                  // Add the point plus a percentage of the vector.
                  vert0ToVert1.mul( distanceAlongVector01 ).add( triangleList[triIter].pt[0] );
                  vert0ToVert2.mul( distanceAlongVector02 ).add( triangleList[triIter].pt[0] );

                  triangleList[triIter].pt[1] = vert0ToVert2;
                  triangleList[triIter].pt[2] = vert0ToVert1;

               }
               else if( insideVertCount == 2 ) 
               { // 2 in  // tesselate into 2 triangles


                  // Copy the triangle\(if it exists) after the current triangle out of
                  // the way so we can override it with the new triangle we're inserting.
                  triangleList[triangleCount] = triangleList[triIter+1];

                  triangleList[triIter].culled = false;
                  triangleList[triIter+1].culled = false;

                  // Get the vector from the outside point into the 2 inside points.
                  Vector vert2ToVert0, vert2ToVert1;
                  vert2ToVert0.setSub( triangleList[triIter].pt[0], triangleList[triIter].pt[2] );
                  vert2ToVert1.setSub( triangleList[triIter].pt[1], triangleList[triIter].pt[2] );

                  // Get the hit point ratio.
                  float hitPointTime_2_0 =  edge - triangleList[triIter].pt[2][component];
                  float distanceAlongVector_2_0 = hitPointTime_2_0 / vert2ToVert0[component];

                  // Calculate the new vert by adding the percentage of the vector plus point 2.
                  vert2ToVert0.mul( distanceAlongVector_2_0 ).add( triangleList[triIter].pt[2] );

                  // Add a new triangle.
                  triangleList[triIter+1].pt[0] = triangleList[triIter].pt[0];
                  triangleList[triIter+1].pt[1] = triangleList[triIter].pt[1];
                  triangleList[triIter+1].pt[2] = vert2ToVert0;

                  //Get the hit point ratio.
                  float hitPointTime_2_1 =  edge - triangleList[triIter].pt[2][component];
                  float distanceAlongVector_2_1 = hitPointTime_2_1 / vert2ToVert1[component];
                  vert2ToVert1.mul( distanceAlongVector_2_1 ).add( triangleList[triIter].pt[2] );
                  triangleList[triIter].pt[0] = triangleList[triIter+1].pt[1];
                  triangleList[triIter].pt[1] = triangleList[triIter+1].pt[2];
                  triangleList[triIter].pt[2] = vert2ToVert1;

                  // Increment triangle count and skip the triangle we just inserted.
                  ++triangleCount;
                  ++triIter;


               }
               else 
               { // all in
                  triangleList[triIter].culled = false;

               }
            }// end if !culled loop            
         }
      }
      for( int index = 0; index < triangleCount; ++index ) 
      {
         if( !triangleList[index].culled ) 
         {
            // Set the near and far plan and the min and max z values respectively.
            for( int vertind = 0; vertind < 3; ++ vertind ) 
            {
               float triangleCoordZ = triangleList[index].pt[vertind].z;
               if( nearPlane > triangleCoordZ ) 
               {
                  nearPlane = triangleCoordZ;
               }
               if( farPlane < triangleCoordZ ) 
               {
                  farPlane = triangleCoordZ;
               }
            }
         }
      }
   }  


   // set the clipping planes on the output bounding box 
   inOutLightFrustumBounds.min.z = nearPlane;
   inOutLightFrustumBounds.max.z = farPlane;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CascadesConfig::CascadesConfig( int numCascades, int numCascadesPerTextureRow, float pcfBlurSize )
   : m_numCascades( numCascades )
   , m_numCascadesPerTextureRow( numCascadesPerTextureRow )
   , m_pcfBlurSize( pcfBlurSize )
{
   m_cascadeIntervals = new float[m_numCascades + 1];
}

///////////////////////////////////////////////////////////////////////////////

CascadesConfig::~CascadesConfig()
{
   delete [] m_cascadeIntervals;
   m_cascadeIntervals = NULL;
}

///////////////////////////////////////////////////////////////////////////////
