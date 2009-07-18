#include "impl-DirectX\D3DDefaultRenderingTarget.h"
#include "impl-DirectX\D3DRenderer.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

D3DDefaultRenderingTarget::D3DDefaultRenderingTarget(IDirect3DDevice9& d3Device,
                                                     D3DRenderer& renderer)
      : m_d3Device(d3Device),
      m_renderer(renderer),
      m_backBuffer(NULL)
{
   m_renderer.attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

D3DDefaultRenderingTarget::~D3DDefaultRenderingTarget()
{
   m_renderer.detachObserver(*this);

   if (m_backBuffer != NULL)
   {
      m_backBuffer->Release();
      m_backBuffer = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DDefaultRenderingTarget::use(unsigned char idx)
{
   if (m_backBuffer == NULL) {return;}

   m_d3Device.SetRenderTarget(idx, m_backBuffer);
   m_d3Device.Clear(idx, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
}

///////////////////////////////////////////////////////////////////////////////

void D3DDefaultRenderingTarget::update(D3DRenderer& renderer)
{
   if (m_backBuffer != NULL)
   {
      m_backBuffer->Release();
      m_backBuffer = NULL;
   }

   if (FAILED(m_d3Device.GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_backBuffer)))
   {
      throw std::runtime_error("Can't obtain an instance of the back buffer");
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DDefaultRenderingTarget::update(D3DRenderer& renderer, const D3DGraphResourceOp& operation)
{
   switch(operation)
   {
   case GRO_RELEASE_RES:
      {
         if (m_backBuffer != NULL)
         {
            m_backBuffer->Release();
            m_backBuffer = NULL;
         }
         break;
      }
   
   case GRO_CREATE_RES:
      {
         if (m_backBuffer != NULL)
         {
            m_backBuffer->Release();
            m_backBuffer = NULL;
         }

         if (FAILED(m_d3Device.GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_backBuffer)))
         {
            throw std::runtime_error("Can't obtain an instance of the back buffer");
         }
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
