#include "dx9-Renderer\DX9LineSegments.h"
#include "dx9-Renderer\DX9Renderer.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

void RCRenderLineSegments::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   DX9LineSegments* segments = dxRenderer.getLineSegments( m_segments );
   if ( segments )
   {
      segments->render();
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

D3DVERTEXELEMENT9 DX9LineSegments::s_vtxDecl[] = 
{
   {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
   {0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
   D3DDECL_END()
};

///////////////////////////////////////////////////////////////////////////////

DX9LineSegments::DX9LineSegments( const DX9Renderer& renderer, const LineSegments& lines )
   : m_renderer( renderer )
   , m_d3Device( renderer.getD3Device() )
   , m_lines( lines )
   , m_vertexDecl(NULL)
   , m_vb(NULL)
   , m_segsCount(0)
{
   m_d3Device.CreateVertexDeclaration( s_vtxDecl, &m_vertexDecl );
   create();
}

///////////////////////////////////////////////////////////////////////////////

DX9LineSegments::~DX9LineSegments()
{
   destroy();

   if (m_vertexDecl != NULL)
   {
      m_vertexDecl->Release();
      m_vertexDecl = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////


void DX9LineSegments::render()
{
   if ( m_vb == NULL )
   {
      return;
   }

   m_d3Device.SetStreamSource( 0, m_vb, 0, sizeof( LineVertex ) );
   m_d3Device.SetVertexDeclaration( m_vertexDecl );
   m_d3Device.DrawPrimitive( D3DPT_LINELIST, 0, m_segsCount );
}

///////////////////////////////////////////////////////////////////////////////

void DX9LineSegments::onLostDevice()
{
   destroy();
}

///////////////////////////////////////////////////////////////////////////////

void DX9LineSegments::onResetDevice()
{
   create();
}

///////////////////////////////////////////////////////////////////////////////

void DX9LineSegments::create()
{
   const std::vector<LineSegment>& segments = m_lines.getSegments();
   m_segsCount = segments.size();
   if (m_segsCount == 0)
   {
      return;
   }

   m_vb = m_renderer.createVertexBuffer( m_segsCount * 2 * sizeof( DX9LineSegments::LineVertex ), 
      0, 
      D3DFVF_XYZ | D3DFVF_DIFFUSE,
      D3DPOOL_MANAGED );

   DX9LineSegments::LineVertex* pVertex = NULL;
   HRESULT res = m_vb->Lock( 0, 0, (void**)&pVertex, 0 );
   if ( FAILED( res) ) 
   { 
      throw std::runtime_error( "Can't lock a vertex buffer" );
   }

   D3DXVECTOR4 tmpColor;
   for (unsigned int i = 0; i < m_segsCount; ++i)
   {
      tmpColor = ( D3DXVECTOR4 )( segments[i].color );

      pVertex->pos = segments[i].start;
      pVertex->color = tmpColor;
      pVertex++;

      pVertex->pos = segments[i].end;
      pVertex->color = tmpColor;
      pVertex++;
   }

   res = m_vb->Unlock();
   if ( FAILED( res ) ) 
   { 
      throw std::runtime_error( "Failed to unlock the vertex buffer" );
   }

}

///////////////////////////////////////////////////////////////////////////////

void DX9LineSegments::destroy()
{
   if ( m_vb != NULL )
   {
      m_vb->Release();
      m_vb = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
