#include "dx9-Renderer\DX9LineSegments.h"
#include "dx9-Renderer\DX9Renderer.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

D3DVERTEXELEMENT9 DX9LineSegments::s_vtxDecl[] = 
{
   {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
   {0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
   D3DDECL_END()
};

///////////////////////////////////////////////////////////////////////////////

DX9LineSegments::DX9LineSegments(LineSegments& lines)
: m_lines(lines)
, m_d3Device(NULL)
, m_renderer(NULL)
, m_vertexDecl(NULL)
, m_vb(NULL)
, m_segsCount(0)
{
}

///////////////////////////////////////////////////////////////////////////////

DX9LineSegments::~DX9LineSegments()
{
   resetVB();

   m_renderer = NULL;
   m_d3Device = NULL;

   if (m_vertexDecl != NULL)
   {
      m_vertexDecl->Release();
      m_vertexDecl = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////


void DX9LineSegments::render()
{
   if ((m_d3Device == NULL) || (m_vb == NULL))
   {
      return;
   }

   m_d3Device->SetStreamSource(0, m_vb, 0, sizeof(LineVertex));
   m_d3Device->SetVertexDeclaration(m_vertexDecl);
   m_d3Device->DrawPrimitive(D3DPT_LINELIST, 0, m_segsCount);
}

///////////////////////////////////////////////////////////////////////////////

void DX9LineSegments::update(const std::vector<LineSegment>& segments)
{
   if ((m_d3Device == NULL) || (m_renderer == NULL))
   {
      return;
   }

   resetVB();
   m_segsCount = segments.size();
   if (m_segsCount == 0)
   {
      return;
   }

   m_vb = m_renderer->createVertexBuffer(m_segsCount * 2 * sizeof(DX9LineSegments::LineVertex), 
      0, 
      0,
      D3DPOOL_MANAGED);

   DX9LineSegments::LineVertex* pVertex = NULL;
   HRESULT res = m_vb->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD);
   if (FAILED(res)) 
   { 
      throw std::logic_error("Can't lock a vertex buffer");
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
   if (FAILED(res)) 
   { 
      throw std::logic_error("Failed to unlock the vertex buffer");
   }

}

///////////////////////////////////////////////////////////////////////////////

void DX9LineSegments::initialize(Renderer& renderer)
{
   m_renderer = dynamic_cast<DX9Renderer*> (&renderer);
   if (m_renderer == NULL)
   {
      throw std::runtime_error("This class works only with DX9Renderer instance");
   }

   m_d3Device = &(m_renderer->getD3Device());
   m_d3Device->CreateVertexDeclaration(s_vtxDecl, &m_vertexDecl);

   resetVB();
}

///////////////////////////////////////////////////////////////////////////////

void DX9LineSegments::resetVB()
{
   if (m_vb != NULL)
   {
      m_vb->Release();
      m_vb = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
