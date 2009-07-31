#include "impl-DirectX\D3DPostProcessEffectRenderable.h"
#include "impl-DirectX\D3DRenderer.h"


///////////////////////////////////////////////////////////////////////////////

DWORD D3DPostProcessEffectRenderable::ScreenVertex::FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

///////////////////////////////////////////////////////////////////////////////

D3DPostProcessEffectRenderable::D3DPostProcessEffectRenderable(IDirect3DDevice9& d3Device,
                                                   D3DRenderer& renderer)
      : PostProcessEffectRenderable(renderer),
      m_d3Device(d3Device),
      m_renderer(renderer),
      m_vb(NULL)
{
   recreateGeometry(m_renderer.getViewportWidth(), m_renderer.getViewportHeight());
}

///////////////////////////////////////////////////////////////////////////////

D3DPostProcessEffectRenderable::~D3DPostProcessEffectRenderable()
{
   releaseQuad();
}

///////////////////////////////////////////////////////////////////////////////

void D3DPostProcessEffectRenderable::render()
{
   if (m_vb == NULL) {return;}

   m_d3Device.SetStreamSource(0, m_vb, 0, sizeof(ScreenVertex));
   m_d3Device.SetFVF(ScreenVertex::FVF);
   m_d3Device.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

///////////////////////////////////////////////////////////////////////////////

void D3DPostProcessEffectRenderable::recreateGeometry(unsigned int backBufferWidth, 
                                                unsigned int backBufferHeight)
{
   releaseQuad();
   
   m_vb = m_renderer.createVertexBuffer(sizeof(ScreenVertex) * 4, 
                                        D3DUSAGE_WRITEONLY, 
                                        ScreenVertex::FVF, 
                                        D3DPOOL_MANAGED);

   ScreenVertex* pVertex = NULL;
   if (FAILED(m_vb->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD)))
   {
      throw std::runtime_error("Cannot lock a vertex buffer");
   }

   // fill in the vertex values
   float width = (float)backBufferWidth - 0.5f;
   float height = (float)backBufferHeight - 0.5f;

   float texWidth = 1.f;
   float texHeight = 1.f;


   *pVertex = ScreenVertex(D3DXVECTOR4(width, -0.5f, 0.0f, 1.0f),
                           D3DXVECTOR2(texWidth, 0.0f));
   ++pVertex;

   *pVertex = ScreenVertex(D3DXVECTOR4(width, height, 0.0f, 1.0f),
                           D3DXVECTOR2(texWidth, texHeight));
   ++pVertex;

   *pVertex = ScreenVertex(D3DXVECTOR4(-0.5f, -0.5f, 0.0f, 1.0f),
                           D3DXVECTOR2(0.0f, 0.0f));
   ++pVertex;

   *pVertex = ScreenVertex(D3DXVECTOR4(-0.5f, height, 0.0f, 1.0f),
                           D3DXVECTOR2(0.0f, texHeight));
   ++pVertex;

   m_vb->Unlock();
}

///////////////////////////////////////////////////////////////////////////////

void D3DPostProcessEffectRenderable::releaseQuad()
{
   if (m_vb != NULL)
   {
      m_vb->Release();
      m_vb = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
