#include "core-Renderer\DirectionalLight.h"
#include "core\BoundingSpace.h"
#include "core\MatrixUtils.h"
#include "core\ResourcesManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Defines.h"
#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\BasicRenderCommands.h"
#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\DepthBuffer.h"
#include "core-Renderer\RenderingView.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\VertexShaderConfigurator.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( DirectionalLight );
   PARENT( Light );
   PROPERTY_EDIT( "Color", Color, m_color );
   PROPERTY_EDIT( "strength", float, m_strength );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

float g_cascadeIntervals[] = { 0.0f, 5.0f / 100.0f, 12.0f / 100.0f, 30.0f / 100.0f, 1.0f };

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::DirectionalLight( const std::string& name )
   : Light( name )
   , m_color(1, 1, 1, 1 )
   , m_strength( 1 )
   , m_lightingShader( NULL )
   , m_shadowDepthMapShader( NULL )
   , m_shadowProjectionPS( NULL )
{
   setBoundingVolume( new BoundingSpace() ); 
   initialize();

   m_visibleGeometry.resize( 1024, NULL );
}

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::~DirectionalLight()
{
   // resources manager will take care of these objects in due time
   m_lightingShader = NULL;
   m_shadowDepthMapShader = NULL;
   m_shadowProjectionPS = NULL;
}


///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::onObjectLoaded()
{
   __super::onObjectLoaded();

   initialize();
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::render( Renderer& renderer, const LightingRenderData& data )
{
   if ( !m_lightingShader )
   {
      return;
   }
   
   // first - render a shadow map, if this light is set to cast shadows
   bool drawShadows = castsShadows();
   if ( drawShadows )
   {
      renderShadowMap( renderer, data );
   }

   // and now render the light contribution
   renderLighting( renderer, data, drawShadows );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::renderLighting( Renderer& renderer, const LightingRenderData& data, bool drawShadows )
{
   if ( !data.m_sceneColorTex )
   {
      return;
   }

   // activate the final render target
   new ( renderer() ) RCActivateRenderTarget( data.m_finalLightColorTarget );

   // set and configure the pixel shader
   RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_lightingShader, renderer );
   {
      const Matrix& globalMtx = getGlobalMtx();
      
      Vector halfPixel;
      LightUtils::calculateHalfPixel( renderer, data.m_depthTex, halfPixel );

      Camera& activeCamera = renderer.getActiveCamera();
      Vector lightDirVS;
      activeCamera.getViewMtx().transformNorm( globalMtx.forwardVec(), lightDirVS );
      psComm->setVec4( "g_halfPixel", halfPixel );
      psComm->setVec4( "g_lightDirVS", lightDirVS );
      psComm->setVec4( "g_lightColor", (const Vector&)m_color );
      psComm->setFloat( "g_strength", m_strength );

      psComm->setTexture( "g_Depth", data.m_depthTex );
      psComm->setTexture( "g_Normals", data.m_normalsTex );
      psComm->setTexture( "g_Speculars", data.m_specularTex );
      psComm->setTexture( "g_SceneColor", data.m_sceneColorTex );

      psComm->setBool( "g_drawShadows", drawShadows );
      if ( data.m_screenSpaceShadowMap )
      {
         psComm->setTexture( "g_ShadowMap", data.m_screenSpaceShadowMap );
      }
   }

   // draw the geometry
   uint quadWidth = renderer.getViewportWidth();
   uint quadHeight = renderer.getViewportHeight();
   new ( renderer() ) RCFullscreenQuad( quadWidth, quadHeight );

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_lightingShader, renderer );
   new ( renderer() ) RCDeactivateRenderTarget();
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::renderShadowMap( Renderer& renderer, const LightingRenderData& data )
{
   if ( !m_castsShadows || !m_shadowDepthMapShader || !m_shadowProjectionPS || !data.m_shadowDepthTexture )
   {
      return;
   }

   Camera& activeCamera = renderer.getActiveCamera();

   const float pcfBlurSize = 3.0f;
   float cascadeDimensions = (float)( data.m_shadowDepthTexture->getWidth() ) / (float)CASCADES_IN_ROW;
   calculateCascadesBounds( activeCamera, pcfBlurSize, cascadeDimensions, data.m_renderingView );

   // set the light camera
   Camera lightCamera( "dirLightCamera", renderer, Camera::PT_ORTHO );
   const Matrix& lightGlobalMtx = getGlobalMtx();
   lightCamera.setLocalMtx( lightGlobalMtx );
   lightCamera.setPosition( Vector::ZERO );

   // render cascades
   renderCascades( renderer, activeCamera, lightCamera, data );

   // combine the cascades
   combineCascades( renderer, data, activeCamera, lightCamera, cascadeDimensions );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::calculateCascadesBounds( Camera& activeCamera, float pcfBlurSize, float cascadeDimensions, const RenderingView* renderingView )
{
   // calculate the camera depth range
   float cameraRange = activeCamera.getFarClippingPlane() - activeCamera.getNearClippingPlane();

   // pre-calculate light camera transformations that we'll use later on during AABBs calculations
   Matrix cameraToLightViewTransform, lightViewMtx;
   {
      Matrix invViewCamera;
      invViewCamera.setInverse( activeCamera.getViewMtx() );
      Matrix lightGlobalMtx = getGlobalMtx();
      lightGlobalMtx.setPosition( Vector::ZERO );

      MatrixUtils::calculateViewMtx( lightGlobalMtx, lightViewMtx );

      cameraToLightViewTransform.setMul( invViewCamera, lightViewMtx );
   }

   // acquire scene's AABB and transform it to the light view space
   Vector sceneBBPointsLightSpace[8];
   {
      const AABoundingBox& sceneBounds = renderingView->getSceneBounds();

      Vector sceneAABBPoints[8];
      createBBPoints( sceneBounds, sceneAABBPoints );

      // Transform the scene bounding box to light space.
      for( int i = 0; i < 8; ++i ) 
      {
         lightViewMtx.transform( sceneAABBPoints[i], sceneBBPointsLightSpace[i] );
      }
   }

   // calculate light frustum bounds
   AABoundingBox cascadeFrustumBounds;
   Vector worldUnitsPerTexel;
   for ( uint i = 0; i < NUM_CASCADES; ++i )
   {
      float frustumIntervalBegin = max( 1.01f, g_cascadeIntervals[i] * cameraRange );
      float frustumIntervalEnd = g_cascadeIntervals[i + 1] * cameraRange;
      m_cascadeConfigs[i].m_cameraNearZ = frustumIntervalBegin;
      m_cascadeConfigs[i].m_cameraFarZ = frustumIntervalEnd;

      calculateCameraCascadeFrustumBounds( activeCamera, frustumIntervalBegin, frustumIntervalEnd, cascadeFrustumBounds );

      // transform the bounds to light
      AABoundingBox& lightFrustumBounds = m_cascadeConfigs[i].m_lightFrustumBounds;
      cascadeFrustumBounds.transform( cameraToLightViewTransform, lightFrustumBounds );

      // now adjust the light frustum a bit in order to remove the shimmering effect
      // of shadow edges whenever the camera is moved
      {
         // We calculate a looser bound based on the size of the PCF blur.  This ensures us that we're 
         // sampling within the correct map.
         float scaleDueToBlurAMT = ( (float)( pcfBlurSize * 2 + 1 ) / cascadeDimensions );
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

      // calculate viewport
      {
         Viewport& viewport = m_cascadeConfigs[i].m_viewport;
         viewport.m_minZ = 0.0f;
         viewport.m_maxZ = 1.0f;
         viewport.m_offsetX = (ulong)( ( i % CASCADES_IN_ROW ) * cascadeDimensions );
         viewport.m_offsetY = (ulong)( ( i / CASCADES_IN_ROW ) * cascadeDimensions );
         viewport.m_width = (ulong)cascadeDimensions;
         viewport.m_height = (ulong)cascadeDimensions;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::calculateCameraCascadeFrustumBounds( Camera& activeCamera, float intervalBegin, float intervalEnd, AABoundingBox& outFrustumPart ) const
{
   float prevNearZ, prevFarZ;
   activeCamera.getClippingPlanes( prevNearZ, prevFarZ );
   activeCamera.setClippingPlanes( intervalBegin, intervalEnd );

   Frustum frustum;
   activeCamera.calculateFrustum( frustum );
   frustum.calculateBoundingBox( outFrustumPart );

   activeCamera.setClippingPlanes( prevNearZ, prevFarZ );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::createBBPoints( const AABoundingBox& inAABB, Vector* outBBPoints ) const
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
   extents.setSub( inAABB.max, inAABB.min );

   Vector center;
   center.setAdd( inAABB.min, inAABB.max ).mul( 0.5f );

   for( int i = 0; i < 8; ++i ) 
   {
      outBBPoints[i].setMulAdd( extents, extentsMap[i], center );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::calculateLightClippingPlanes( const Vector* sceneBBInLightSpace, AABoundingBox& inOutLightFrustumBounds ) const
{
   struct Triangle 
   {
      Vector pt[3];
      BOOL culled;
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
      triangleList[0].culled = FALSE;

      // Clip each invidual triangle against the 4 frustums.  When ever a triangle is clipped into new triangles, 
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
                  for( int triPtIter=0; triPtIter < 3; ++triPtIter ) 
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
                  for( INT triPtIter=0; triPtIter < 3; ++triPtIter ) 
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
                  for( INT triPtIter=0; triPtIter < 3; ++triPtIter ) 
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
                  for( INT triPtIter=0; triPtIter < 3; ++triPtIter ) 
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

               // Move the points that pass the frustum test to the begining of the array.
               if( pointPassesCollision[1] && !pointPassesCollision[0] ) 
               {
                  tempOrder =  triangleList[triIter].pt[0];   
                  triangleList[triIter].pt[0] = triangleList[triIter].pt[1];
                  triangleList[triIter].pt[1] = tempOrder;
                  pointPassesCollision[0] = TRUE;            
                  pointPassesCollision[1] = FALSE;            
               }
               if( pointPassesCollision[2] && !pointPassesCollision[1] ) 
               {
                  tempOrder =  triangleList[triIter].pt[1];   
                  triangleList[triIter].pt[1] = triangleList[triIter].pt[2];
                  triangleList[triIter].pt[2] = tempOrder;
                  pointPassesCollision[1] = TRUE;            
                  pointPassesCollision[2] = FALSE;                        
               }
               if( pointPassesCollision[1] && !pointPassesCollision[0] ) 
               {
                  tempOrder =  triangleList[triIter].pt[0];   
                  triangleList[triIter].pt[0] = triangleList[triIter].pt[1];
                  triangleList[triIter].pt[1] = tempOrder;
                  pointPassesCollision[0] = TRUE;            
                  pointPassesCollision[1] = FALSE;            
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
                  FLOAT hitPointTimeRatio = edge - triangleList[triIter].pt[0][component];

                  // Calculate the distance along the vector as ratio of the hit ratio to the component.
                  FLOAT distanceAlongVector01 = hitPointTimeRatio / vert0ToVert1[component];
                  FLOAT distanceAlongVector02 = hitPointTimeRatio / vert0ToVert2[component];

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


void DirectionalLight::renderCascades( Renderer& renderer, Camera& activeCamera, Camera& lightCamera, const LightingRenderData& data )
{
   if ( !data.m_shadowDepthTexture || !data.m_shadowDepthSurface )
   {
      return;
   }

   // bind the shader and set the render target
   new ( renderer() ) RCActivateRenderTarget( data.m_shadowDepthTexture );
   new ( renderer() ) RCActivateDepthBuffer( data.m_shadowDepthSurface );
   new ( renderer() ) RCBindPixelShader( *m_shadowDepthMapShader, renderer );
   new ( renderer() ) RCClearDepthBuffer();

   // render cascades
   AABoundingBox expandedCascadeBounds;
   for ( uint cascadeIdx = 0; cascadeIdx < NUM_CASCADES; ++cascadeIdx )
   {
      const AABoundingBox& cascadeBounds = m_cascadeConfigs[cascadeIdx].m_lightFrustumBounds;
      lightCamera.setNearPlaneDimensions( cascadeBounds.max.x - cascadeBounds.min.x, cascadeBounds.max.y - cascadeBounds.min.y );
      lightCamera.setClippingPlanes( cascadeBounds.min.z, cascadeBounds.max.z );

      new ( renderer() ) RCSetViewport( m_cascadeConfigs[cascadeIdx].m_viewport );

      // render the scene with the new camera
      {
         VSSetter vsSetter( lightCamera );

         expandedCascadeBounds.setExpanded( cascadeBounds, 2.0f );
         m_visibleGeometry.clear();
         data.m_renderingView->collectRenderables( cascadeBounds, m_visibleGeometry );

         uint sceneElemsCount = m_visibleGeometry.size();
         for ( uint i = 0; i < sceneElemsCount; ++i )
         {
            m_visibleGeometry[i]->render( renderer, &vsSetter);
         }
      }
   }

   // clean up
   new ( renderer() ) RCResetViewport();
   new ( renderer() ) RCUnbindPixelShader( *m_shadowDepthMapShader, renderer );
   new ( renderer() ) RCDeactivateDepthBuffer( data.m_shadowDepthSurface );
   new ( renderer() ) RCDeactivateRenderTarget();
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::combineCascades( Renderer& renderer, const LightingRenderData& data, Camera& activeCamera, Camera& lightCamera, float cascadeDimensions )
{
   if ( !data.m_screenSpaceShadowMap )
   {
      return;
   }

   float depthRanges[NUM_CASCADES + 1];
   Vector viewportOffsets[NUM_CASCADES];
   Matrix clipToLightSpaceMtx[NUM_CASCADES];

   float shadowMapDimension = (float)data.m_shadowDepthTexture->getWidth();

   Matrix worldToClipSpace;
   worldToClipSpace.setMul( activeCamera.getViewMtx(), activeCamera.getProjectionMtx() );

   Matrix invClipSpace;
   invClipSpace.setInverse( worldToClipSpace );

   for( uint i = 0; i < NUM_CASCADES; ++i )
   {
      CascadeConfig& config = m_cascadeConfigs[i];
      depthRanges[i] = config.m_cameraFarZ;
      viewportOffsets[i].set( (float)config.m_viewport.m_offsetX / shadowMapDimension, (float)config.m_viewport.m_offsetY / shadowMapDimension, 0 );

      // calculate the transformation matrix
      {
         const AABoundingBox& cascadeBounds = m_cascadeConfigs[i].m_lightFrustumBounds;
         lightCamera.setNearPlaneDimensions( cascadeBounds.max.x - cascadeBounds.min.x, cascadeBounds.max.y - cascadeBounds.min.y );
         lightCamera.setClippingPlanes( cascadeBounds.min.z, cascadeBounds.max.z );

         Matrix lightViewProj;
         lightViewProj.setMul( lightCamera.getViewMtx(), lightCamera.getProjectionMtx() );

         clipToLightSpaceMtx[i].setMul( invClipSpace, lightViewProj );     
      }
   }
   depthRanges[NUM_CASCADES] = FLT_MAX;

   // again, clean the depth buffer
   new ( renderer() ) RCClearDepthBuffer();

   //  render the shadow projection map
   {
      new ( renderer() ) RCActivateRenderTarget( data.m_screenSpaceShadowMap );
      RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_shadowProjectionPS, renderer );
      {
         // set the shadow map
         float texelDimension = 1.0f / cascadeDimensions;
         float cascadeScale = cascadeDimensions / shadowMapDimension;
         psComm->setFloat( "g_texelDimension", texelDimension );
         psComm->setTexture( "g_shadowDepthMap", data.m_shadowDepthTexture );
         psComm->setFloat( "g_cascadeScale", cascadeScale );
         psComm->setFloat( "g_cascadeDepthRanges", depthRanges, NUM_CASCADES + 1 );
         psComm->setVec4( "g_cascadeOffsets", viewportOffsets, NUM_CASCADES  );
         psComm->setMtx( "g_clipToLightSpaceMtx", clipToLightSpaceMtx , NUM_CASCADES );
      }
    
      // render visible scene elements
      uint sceneElemsCount = data.m_geometryToRender->size();
      for ( uint i = 0; i < sceneElemsCount; ++i )
      {
         (*data.m_geometryToRender)[i]->render( renderer );
      }

      new ( renderer() ) RCUnbindPixelShader( *m_shadowProjectionPS, renderer );
      new ( renderer() ) RCDeactivateRenderTarget();
   }
   
   // clean the depth buffer once we're done
   new ( renderer() ) RCClearDepthBuffer();
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::initialize()
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   // lighting shaders
   {
      FilePath psPath( LIGHTING_SHADERS_DIR "Lights/directionalLight.tpsh" );
      m_lightingShader = resMgr.create< PixelShader >( psPath, true );
   }

   // shadow shaders
   {
      FilePath shadowDepthMapShaderPath( LIGHTING_SHADERS_DIR "Shadows/DirectionalLight/shadowDepthMapShader.tpsh" );
      m_shadowDepthMapShader = resMgr.create< PixelShader >( shadowDepthMapShaderPath, true );

      FilePath shadowProjectionPSPath( LIGHTING_SHADERS_DIR "Shadows/DirectionalLight/shadowProjectionShader.tpsh" );
      m_shadowProjectionPS = resMgr.create< PixelShader >( shadowProjectionPSPath, true );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DirectionalLight::VSSetter::VSSetter( Camera& lightCamera )
{
   m_lightViewProjMtx.setMul( lightCamera.getViewMtx(), lightCamera.getProjectionMtx() );
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::VSSetter::configure( const Geometry& geometry, RCBindVertexShader* command )
{
   const Matrix& geometryWorldMtx = geometry.getGlobalMtx();

   Matrix lightViewProjMtx;
   lightViewProjMtx.setMul( geometryWorldMtx, m_lightViewProjMtx );

   command->setMtx( "g_matLightViewProj", lightViewProjMtx );
}

///////////////////////////////////////////////////////////////////////////////
