#include "dx9-Renderer\DX9LineSegments.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core\Assert.h"


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

DX9LineSegments::DX9LineSegments( const DX9Renderer& renderer, const LineSegments& lines )
   : m_renderer( renderer )
   , m_d3Device( renderer.getD3Device() )
   , m_lines( lines )
   , m_vb(NULL)
   , m_segsCount(0)
{
   create();
}

///////////////////////////////////////////////////////////////////////////////

DX9LineSegments::~DX9LineSegments()
{
   destroy();

}

///////////////////////////////////////////////////////////////////////////////


void DX9LineSegments::render()
{
   if ( m_vb == NULL )
   {
      return;
   }

   m_d3Device.SetStreamSource( 0, m_vb, 0, sizeof( LineVertex ) );
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
      ASSERT_MSG( false, "Can't lock a vertex buffer" );
      return;
   }

   D3DXVECTOR4 tmpColor;
   for (unsigned int i = 0; i < m_segsCount; ++i)
   {
      tmpColor = ( const D3DXVECTOR4& )( segments[i].color );

      pVertex->pos = ( const D3DXVECTOR3& )segments[i].start;
      pVertex->color = ( const D3DXVECTOR4& )tmpColor;
      pVertex++;

      pVertex->pos = ( const D3DXVECTOR3& )segments[i].end;
      pVertex->color = ( const D3DXVECTOR4& )tmpColor;
      pVertex++;
   }

   res = m_vb->Unlock();
   if ( FAILED( res ) ) 
   { 
      ASSERT_MSG( false, "Failed to unlock the vertex buffer" );
      return;
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
