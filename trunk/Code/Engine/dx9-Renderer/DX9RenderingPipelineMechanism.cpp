#include "dx9-Renderer\DX9RenderingPipelineMechanism.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "dx9-Renderer\DXErrorParser.h"
#include "core-Renderer/Camera.h"
#include "core\Color.h"
#include <stdexcept>

#define _USE_MATH_DEFINES
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

DX9RenderingPipelineMechanism::DX9RenderingPipelineMechanism( RenderingPipelineMechanism& renderingPipelineMechanism )
   : m_renderingPipelineMechanism( renderingPipelineMechanism )
   , m_d3Device( NULL )
   , m_linesBuffer( NULL )
   , DEBUG_LINES_MAX_COUNT( 65535 )
   , m_pVertex( NULL )
   , m_linesCount( 0 )
{
   D3DXMatrixIdentity( &m_identityMtx );
}

///////////////////////////////////////////////////////////////////////////////

DX9RenderingPipelineMechanism::~DX9RenderingPipelineMechanism()
{
   if ( m_linesBuffer )
   {
      m_linesBuffer->Release();
      m_linesBuffer = NULL;
   }

   m_d3Device = NULL;
   m_pVertex = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::initialize( Renderer& renderer )
{
   DX9Renderer* dxRenderer = dynamic_cast< DX9Renderer* >( &renderer );
   if ( dxRenderer == NULL )
   {
      throw std::runtime_error( "This implementation can work only with DX9Renderer" );
   }

   m_d3Device = &dxRenderer->getD3Device();

   m_linesBuffer = dxRenderer->createVertexBuffer( DEBUG_LINES_MAX_COUNT * sizeof( DebugVertex ), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED );
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::passBegin()
{
   if ( m_d3Device )
   {
      m_d3Device->BeginScene();
   }

   // reset the debug data
   m_linesCount = 0;
   ASSERT_MSG( m_pVertex == NULL, "Debug lines VB wasn't properly unlocked" );
   m_linesBuffer->Lock( 0, DEBUG_LINES_MAX_COUNT * sizeof( DebugVertex ), (void**)&m_pVertex, 0 );
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::passEnd()
{
   ASSERT_MSG( m_pVertex == NULL, "Debug lines VB wasn't properly unlocked" );

   if ( m_d3Device )
   {
      m_d3Device->EndScene();
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::renderDebug( Camera& activeCamera )
{
   // set transformations
   const D3DXMATRIX& projectionMtx = activeCamera.getProjectionMtx();
   const D3DXMATRIX& viewMtx = activeCamera.getViewMtx();

   m_d3Device->SetTransform( D3DTS_WORLD, &m_identityMtx );
   m_d3Device->SetTransform( D3DTS_PROJECTION, &projectionMtx );
   m_d3Device->SetTransform( D3DTS_VIEW, &viewMtx );

   // unlock the lines buffer
   m_linesBuffer->Unlock();
   m_pVertex = NULL;

   // draw the lines
   m_d3Device->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
   m_d3Device->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1 );
   m_d3Device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
   m_d3Device->SetStreamSource( 0, m_linesBuffer, 0, sizeof( DebugVertex ) );
   m_d3Device->DrawPrimitive( D3DPT_LINELIST, 0, m_linesCount );

   // cleanup
   m_d3Device->SetRenderState( D3DRS_AMBIENT, 0 );
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::addLineVertex( const D3DXVECTOR3& vtx, const Color& color )
{
   m_pVertex->m_vtx = vtx;
   m_pVertex->m_color = D3DCOLOR_COLORVALUE( color.r, color.g, color.b, color.a );
   ++m_pVertex;
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::drawLine( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color )
{   
   addLineVertex( start, color );
   addLineVertex( end, color );
   m_linesCount += 1;
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::drawArc( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color )
{
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::drawArrow( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color )
{

}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::drawBox( const D3DXMATRIX& transform, const D3DXVECTOR3& size, const Color& color )
{
   static D3DXVECTOR3 vertices[8];
   static D3DXVECTOR3 transformedVertices[8];

   // define the box vertices
   // bottom face
   vertices[0].x = -size.x; vertices[0].y = -size.y; vertices[0].z = -size.z; 
   vertices[1].x =  size.x; vertices[1].y = -size.y; vertices[1].z = -size.z; 
   vertices[2].x =  size.x; vertices[2].y = -size.y; vertices[2].z =  size.z; 
   vertices[3].x = -size.x; vertices[3].y = -size.y; vertices[3].z =  size.z;
   
   // top face
   vertices[4].x = -size.x; vertices[4].y =  size.y; vertices[4].z = -size.z; 
   vertices[5].x =  size.x; vertices[5].y =  size.y; vertices[5].z = -size.z; 
   vertices[6].x =  size.x; vertices[6].y =  size.y; vertices[6].z =  size.z; 
   vertices[7].x = -size.x; vertices[7].y =  size.y; vertices[7].z =  size.z;

   // transform the vertices
   for ( char i = 0; i < 8; ++i )
   {
      D3DXVec3TransformCoord( &transformedVertices[i], &vertices[i], &transform );
   }

   // add the lines:
   // bottom face
   addLineVertex( transformedVertices[0], color );
   addLineVertex( transformedVertices[1], color );
   addLineVertex( transformedVertices[1], color );
   addLineVertex( transformedVertices[2], color );
   addLineVertex( transformedVertices[2], color );
   addLineVertex( transformedVertices[3], color );
   addLineVertex( transformedVertices[3], color );
   addLineVertex( transformedVertices[0], color );
   
   // top face
   addLineVertex( transformedVertices[4], color );
   addLineVertex( transformedVertices[5], color );
   addLineVertex( transformedVertices[5], color );
   addLineVertex( transformedVertices[6], color );
   addLineVertex( transformedVertices[6], color );
   addLineVertex( transformedVertices[7], color );
   addLineVertex( transformedVertices[7], color );
   addLineVertex( transformedVertices[4], color );

   // side lines
   addLineVertex( transformedVertices[0], color );
   addLineVertex( transformedVertices[4], color );
   addLineVertex( transformedVertices[1], color );
   addLineVertex( transformedVertices[5], color );
   addLineVertex( transformedVertices[2], color );
   addLineVertex( transformedVertices[6], color );
   addLineVertex( transformedVertices[3], color );
   addLineVertex( transformedVertices[7], color );

   m_linesCount += 12;
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::drawSphere( const D3DXMATRIX& transform, float radius, const Color& color )
{

}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::drawCylinder( const D3DXMATRIX& transform, float radius, float height, const Color& color )
{

}

///////////////////////////////////////////////////////////////////////////////
