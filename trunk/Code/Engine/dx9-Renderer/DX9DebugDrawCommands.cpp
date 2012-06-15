#include "core-Renderer\DebugDrawCommands.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core-Renderer\Camera.h"
#include "core\Color.h"
#include "core\Math.h"


///////////////////////////////////////////////////////////////////////////////

void RCDrawDebugLine::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   dxRenderer.addDebugLine( m_start, m_end, m_color );
}

///////////////////////////////////////////////////////////////////////////////

void RCDrawDebugArc::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
}

///////////////////////////////////////////////////////////////////////////////

void RCDrawDebugArrow::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );

   Vector dir;
   dir.setSub( m_end, m_start ).normalize();

   Vector perpVec1, perpVec2;
   VectorUtil::calculatePerpendicularVector( dir, perpVec1 );
   perpVec1.normalize();
   perpVec2.setCross( dir, perpVec1 );

   perpVec1.mul( m_lineWidth );
   perpVec2.mul( m_lineWidth );


   // draw the box
   {
      Vector vertices[8];
      vertices[0].setAdd( m_start, perpVec1 );
      vertices[1].setSub( m_start, perpVec1 );
      vertices[2].setAdd( m_start, perpVec2 );
      vertices[3].setSub( m_start, perpVec2 );

      // top face
      vertices[4].setAdd( m_end, perpVec1 );
      vertices[5].setSub( m_end, perpVec1 );
      vertices[6].setAdd( m_end, perpVec2 );
      vertices[7].setSub( m_end, perpVec2 );

      // add the lines:
      // bottom face
      dxRenderer.addDebugTriangle( vertices[0], vertices[2], vertices[1], m_color, m_overlay );
      dxRenderer.addDebugTriangle( vertices[0], vertices[3], vertices[2], m_color, m_overlay );

      // top face
      dxRenderer.addDebugTriangle( vertices[4], vertices[6], vertices[5], m_color, m_overlay );
      dxRenderer.addDebugTriangle( vertices[4], vertices[7], vertices[6], m_color, m_overlay );

      // right face
      dxRenderer.addDebugTriangle( vertices[0], vertices[5], vertices[1], m_color, m_overlay );
      dxRenderer.addDebugTriangle( vertices[0], vertices[4], vertices[5], m_color, m_overlay );

      // left face
      dxRenderer.addDebugTriangle( vertices[7], vertices[2], vertices[6], m_color, m_overlay );
      dxRenderer.addDebugTriangle( vertices[7], vertices[3], vertices[2], m_color, m_overlay );

      // front face
      dxRenderer.addDebugTriangle( vertices[1], vertices[6], vertices[2], m_color, m_overlay );
      dxRenderer.addDebugTriangle( vertices[1], vertices[5], vertices[6], m_color, m_overlay );

      // back face
      dxRenderer.addDebugTriangle( vertices[4], vertices[3], vertices[7], m_color, m_overlay );
      dxRenderer.addDebugTriangle( vertices[4], vertices[0], vertices[3], m_color, m_overlay );
   }

   // draw the arrow tip as a cone
   {
      Vector vertices[5];
      const float tipSizeMultiplier = 6;
      perpVec1.mul( tipSizeMultiplier );
      perpVec2.mul( tipSizeMultiplier );
      vertices[0].setAdd( m_end, perpVec1 );
      vertices[1].setSub( m_end, perpVec1 );
      vertices[2].setAdd( m_end, perpVec2 );
      vertices[3].setSub( m_end, perpVec2 );
      vertices[4].setMulAdd( dir, m_lineWidth * tipSizeMultiplier * 2, m_end );

      // cone bottom
      dxRenderer.addDebugTriangle( vertices[0], vertices[2], vertices[1], m_color, m_overlay );
      dxRenderer.addDebugTriangle( vertices[0], vertices[3], vertices[2], m_color, m_overlay );

      // cone top
      dxRenderer.addDebugTriangle( vertices[0], vertices[1], vertices[4], m_color, m_overlay );
      dxRenderer.addDebugTriangle( vertices[1], vertices[2], vertices[4], m_color, m_overlay );
      dxRenderer.addDebugTriangle( vertices[2], vertices[3], vertices[4], m_color, m_overlay );
      dxRenderer.addDebugTriangle( vertices[3], vertices[0], vertices[4], m_color, m_overlay );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RCDrawDebugBox::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );

   Vector vertices[8];
   Vector transformedVertices[8];

   // define the box vertices
   // bottom face
   vertices[0].x = -m_size.x; vertices[0].y = -m_size.y; vertices[0].z = -m_size.z; 
   vertices[1].x =  m_size.x; vertices[1].y = -m_size.y; vertices[1].z = -m_size.z; 
   vertices[2].x =  m_size.x; vertices[2].y = -m_size.y; vertices[2].z =  m_size.z; 
   vertices[3].x = -m_size.x; vertices[3].y = -m_size.y; vertices[3].z =  m_size.z;

   // top face
   vertices[4].x = -m_size.x; vertices[4].y =  m_size.y; vertices[4].z = -m_size.z; 
   vertices[5].x =  m_size.x; vertices[5].y =  m_size.y; vertices[5].z = -m_size.z; 
   vertices[6].x =  m_size.x; vertices[6].y =  m_size.y; vertices[6].z =  m_size.z; 
   vertices[7].x = -m_size.x; vertices[7].y =  m_size.y; vertices[7].z =  m_size.z;

   // transform the vertices
   for ( char i = 0; i < 8; ++i )
   {
      m_transform.transform( vertices[i], transformedVertices[i] );
   }

   // add the lines:
   // bottom face
   dxRenderer.addDebugTriangle( transformedVertices[0], transformedVertices[2], transformedVertices[1], m_color, m_overlay );
   dxRenderer.addDebugTriangle( transformedVertices[0], transformedVertices[3], transformedVertices[2], m_color, m_overlay );
   
   // top face
   dxRenderer.addDebugTriangle( transformedVertices[4], transformedVertices[6], transformedVertices[5], m_color, m_overlay );
   dxRenderer.addDebugTriangle( transformedVertices[4], transformedVertices[7], transformedVertices[6], m_color, m_overlay );

   // right face
   dxRenderer.addDebugTriangle( transformedVertices[0], transformedVertices[5], transformedVertices[1], m_color, m_overlay );
   dxRenderer.addDebugTriangle( transformedVertices[0], transformedVertices[4], transformedVertices[5], m_color, m_overlay );

   // left face
   dxRenderer.addDebugTriangle( transformedVertices[7], transformedVertices[2], transformedVertices[6], m_color, m_overlay );
   dxRenderer.addDebugTriangle( transformedVertices[7], transformedVertices[3], transformedVertices[2], m_color, m_overlay );

   // front face
   dxRenderer.addDebugTriangle( transformedVertices[1], transformedVertices[6], transformedVertices[2], m_color, m_overlay );
   dxRenderer.addDebugTriangle( transformedVertices[1], transformedVertices[5], transformedVertices[6], m_color, m_overlay );

   // back face
   dxRenderer.addDebugTriangle( transformedVertices[4], transformedVertices[3], transformedVertices[7], m_color, m_overlay );
   dxRenderer.addDebugTriangle( transformedVertices[4], transformedVertices[0], transformedVertices[3], m_color, m_overlay );
}

///////////////////////////////////////////////////////////////////////////////

void RCDrawDebugRing::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );

   const Vector& origin = m_transform.position();
   const Vector& mainAxis = m_transform.forwardVec();
   const Vector& sideAxis = m_transform.sideVec();
   Vector circumferenceAxis = m_transform.upVec();
   
   const uint segmentsCount = 12;
   const uint segmentVerticesCount = 3;
   
   // calculate torus vertices
   const uint verticesCount = segmentsCount * segmentVerticesCount;
   Vector* vertices = new Vector[verticesCount];
   {
      const float dMainAngle = DEG2RAD( 360.0f / (float)segmentsCount );
      const float dSegmentAngle = DEG2RAD( 360.0f / (float)segmentVerticesCount );
      float mainAngle = 0.0f;
  
      Quaternion mainRot, circumferenceRot;
      Vector vtxPos, posOnCircumference, radiusDisplacement, radiusVec;
      radiusVec.setMul( sideAxis, m_radius );
      uint vtxIdx = 0;
      for ( uint segmentIdx = 0; segmentIdx < segmentsCount; ++segmentIdx, mainAngle += dMainAngle )
      {
         mainRot.setAxisAngle( mainAxis, mainAngle );
         mainRot.transform( radiusVec, radiusDisplacement );

         float segmentAngle = 0.0f;
         for ( uint segVtxIdx = 0; segVtxIdx < segmentVerticesCount; ++segVtxIdx, segmentAngle += dSegmentAngle )
         {
            circumferenceRot.setAxisAngle( circumferenceAxis, segmentAngle );
            Quaternion rotQ;
            rotQ.setMul( circumferenceRot, mainRot );

            // first - create a point on the circumference of the toruses' segment
            vtxPos.setMul( sideAxis, m_circumferenceWidth );
            rotQ.transform( vtxPos, posOnCircumference );

            // and displace the circumference point so that it ends up in its final position on the toruses' circumference
            vertices[vtxIdx].setAdd( radiusDisplacement, posOnCircumference );
            vertices[vtxIdx].add( origin );

            ++vtxIdx;
         }
      }
   }

   // set torus indices
   const uint indicesCount = verticesCount * 6;
   word* indices = new word[indicesCount];
   {
      uint idx = 0;
      for ( uint segmentIdx = 0; segmentIdx < segmentsCount; ++segmentIdx )
      {
         uint currSegmentFirstVtx = segmentIdx * segmentVerticesCount;
         uint nextSegmentFirstVtx = ( currSegmentFirstVtx + segmentVerticesCount ) % verticesCount;

         for ( uint segVtxIdx = 0; segVtxIdx < segmentVerticesCount; ++segVtxIdx )
         {
            uint skipOffset = 0;
            if ( segVtxIdx == 0 )
            {
               skipOffset = segmentVerticesCount;
            }

            indices[ idx++ ] = currSegmentFirstVtx + segVtxIdx - 1 + skipOffset;
            indices[ idx++ ] = currSegmentFirstVtx + segVtxIdx;
            indices[ idx++ ] = nextSegmentFirstVtx + segVtxIdx - 1 + skipOffset;


            indices[ idx++ ] = currSegmentFirstVtx + segVtxIdx;
            indices[ idx++ ] = nextSegmentFirstVtx + segVtxIdx;
            indices[ idx++ ] = nextSegmentFirstVtx + segVtxIdx - 1 + skipOffset;
         }
      }
   }
   // add the mesh to draw
   dxRenderer.addIndexedMesh( vertices, verticesCount, indices, indicesCount, m_color, m_overlay );

   // cleanup
   delete [] indices;
   delete [] vertices;
}

///////////////////////////////////////////////////////////////////////////////

void RCDrawDebugSphere::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
}

///////////////////////////////////////////////////////////////////////////////

void RCDrawDebugCylinder::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
}

///////////////////////////////////////////////////////////////////////////////

void RCDrawDebugGrid::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );

   const float dim = m_gridLinesSpacing <= 0 ? 0 : ( m_gridSize / m_gridLinesSpacing );
   float varPos;
   static Color gridColor( 100, 100, 255 );

   for ( float i = -dim; i <= dim; ++i )
   {
      varPos = i * m_gridLinesSpacing;
      dxRenderer.addDebugLine( Vector( -m_gridSize, 0, varPos ), Vector( m_gridSize, 0, varPos ), gridColor );
      dxRenderer.addDebugLine( Vector( varPos, 0, -m_gridSize ), Vector( varPos, 0, m_gridSize ), gridColor );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RCBeginDebugScene::execute( Renderer& renderer )
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void RCEndDebugScene::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   dxRenderer.flushDebugScene();
}

///////////////////////////////////////////////////////////////////////////////
