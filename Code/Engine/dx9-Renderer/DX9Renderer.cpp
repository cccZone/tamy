#include "dx9-Renderer\DX9Renderer.h"
#include "dx9-Renderer\DXErrorParser.h"
#include <stdexcept>
#include <string>
#include <cassert>

// implementations
#include "core-Renderer\TriangleMesh.h"
#include "dx9-Renderer\DX9TriangleMesh.h"
#include "core-Renderer\LineSegments.h"
#include "dx9-Renderer\DX9LineSegments.h"
#include "core-Renderer\Texture.h"
#include "dx9-Renderer\DX9Texture.h"
#include "core-Renderer\RenderTarget.h"
#include "dx9-Renderer\DX9RenderTarget.h"
#include "core-Renderer\EffectShader.h"
#include "dx9-Renderer\DX9EffectShader.h"
#include "core-Renderer\Font.h"
#include "dx9-Renderer\DX9Font.h"
#include "core-Renderer\RenderingPass.h"
#include "dx9-Renderer\DX9RenderingPass.h"
#include "core-Renderer\VertexShader.h"
#include "dx9-Renderer\DX9VertexShader.h"
#include "core-Renderer\PixelShader.h"
#include "dx9-Renderer\DX9PixelShader.h"
#include "core-Renderer\Skeleton.h"
#include "dx9-Renderer\DX9Skeleton.h"
#include "core-Renderer\FullscreenQuad.h"
#include "dx9-Renderer\DX9FullscreenQuad.h"
#include "core-Renderer\RenderingPipelineMechanism.h"
#include "dx9-Renderer\DX9RenderingPipelineMechanism.h"

/////////////////////////////////////////////////////////////////////////////

DX9Renderer::DX9Renderer( IDirect3D9& d3d9,
                          IDirect3DDevice9* d3Device, 
                          const DX9Settings& settings,
                          D3DPRESENT_PARAMETERS creationParams,
                          UINT frontBufferWidth,
                          UINT frontBufferHeight,
                          bool hardwareTLOn)
      : Renderer(frontBufferWidth, frontBufferHeight)
      , m_d3d9( d3d9 )
      , m_d3Device(d3Device)
      , m_settings( settings )
      , m_creationParams(creationParams)
      , m_deviceLost(false)
      , m_hardwareTLOn(hardwareTLOn)
{
   m_viewport.X = 0;
   m_viewport.Y = 0;
   m_viewport.Width = frontBufferWidth;
   m_viewport.Height = frontBufferHeight;
   m_viewport.MinZ = 0.0f;
   m_viewport.MaxZ = 1.0f;

   // associate implementations
   associate< TriangleMesh, DX9TriangleMesh > ();
   associate< LineSegments, DX9LineSegments > ();
   associate< Texture, DX9Texture > ();
   associate< RenderTarget, DX9RenderTarget > ();
   associate< EffectShader, DX9EffectShader > ();
   associate< Font, DX9Font > ();
   associate< RenderingPass, DX9RenderingPass > ();
   associate< VertexShader, DX9VertexShader > ();
   associate< PixelShader, DX9PixelShader > ();
   associate< Skeleton, DX9Skeleton > ();
   associate< FullscreenQuad, DX9FullscreenQuad > ();
   associate< RenderingPipelineMechanism, DX9RenderingPipelineMechanism > ();

   // sample texture formats
   sampleOptimalTextureFormats();
}

/////////////////////////////////////////////////////////////////////////////

DX9Renderer::~DX9Renderer()
{
   if (m_d3Device != NULL)
   {
      m_d3Device->Release();
      m_d3Device = NULL;
   }
}

/////////////////////////////////////////////////////////////////////////////

void DX9Renderer::initRenderer()
{
   // Setup our D3D Device initial states
   m_d3Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

   m_d3Device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
   m_d3Device->SetViewport(&m_viewport);

   Subject<DX9Renderer, DX9GraphResourceOp>::notify(GRO_CREATE_RES);
}

/////////////////////////////////////////////////////////////////////////////

void DX9Renderer::resetViewport(unsigned int width, unsigned int height)
{
   if ((m_d3Device) && (m_creationParams.Windowed == TRUE))
   {
      // Store new sizes
      m_creationParams.BackBufferWidth = width;
      m_creationParams.BackBufferHeight = height;

      m_viewport.X = 0;
      m_viewport.Y = 0;
      m_viewport.Width = width;
      m_viewport.Height = height;
      m_viewport.MinZ = 0.0f;
      m_viewport.MaxZ = 1.0f;

      // Reset the device
      D3DPRESENT_PARAMETERS creationParams = m_creationParams;
      if (creationParams.Windowed) {creationParams.BackBufferFormat = D3DFMT_UNKNOWN;}
      creationParams.BackBufferCount = 0;
      creationParams.BackBufferWidth = 0;
      creationParams.BackBufferHeight = 0;

      Subject<DX9Renderer, DX9GraphResourceOp>::notify(GRO_RELEASE_RES);

      if (FAILED(m_d3Device->Reset(&creationParams)))
      {
         throw std::runtime_error("Could not reset the graphical device");
      }

      m_creationParams = creationParams;
   }
   initRenderer();
}

/////////////////////////////////////////////////////////////////////////////

void DX9Renderer::renderingBegin()
{
}

/////////////////////////////////////////////////////////////////////////////

void DX9Renderer::renderingEnd()
{
   if ( FAILED( m_d3Device->Present( NULL, NULL, NULL, NULL ) ) )
   {
      m_deviceLost = true;
   }
}

/////////////////////////////////////////////////////////////////////////////

bool DX9Renderer::isGraphicsSystemReady() const
{
   return !(m_deviceLost == true);
}

/////////////////////////////////////////////////////////////////////////////

void DX9Renderer::attemptToRecoverGraphicsSystem()
{
   HRESULT deviceStatus = m_d3Device->TestCooperativeLevel();

   switch(deviceStatus)
   {
   case D3D_OK:
      {
         m_deviceLost = false;
         break;
      }

   case D3DERR_DEVICENOTRESET:
      {
         HRESULT resettingResult = m_d3Device->Reset(&m_creationParams);

         if (SUCCEEDED(resettingResult))
         {
            m_deviceLost = false;
         }
         break;
      }

   default:
      break;
   }

   if (m_deviceLost == false)
   {
      initRenderer();
   }
}

/////////////////////////////////////////////////////////////////////////////

void DX9Renderer::activateRenderTarget( RenderTarget* renderTarget )
{
   IDirect3DSurface9* renderTargetSurface = NULL;

   if ( renderTarget )
   {
      // rendering target is set - use it
      IDirect3DTexture9* rtTexture = reinterpret_cast< IDirect3DTexture9* >( renderTarget->getPlatformSpecific() );
      HRESULT res = rtTexture->GetSurfaceLevel( 0, &renderTargetSurface );
      if ( FAILED( res ) || !renderTargetSurface )
      {
         std::string errorMsg = translateDxError( "Could not acquire a render target surface", res );
         throw std::runtime_error( errorMsg );
      }
   }
   else
   {
      // we're gonna be using the back buffer
      m_d3Device->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &renderTargetSurface );
   }

   m_d3Device->SetRenderTarget( 0, renderTargetSurface );
   renderTargetSurface->Release();
}

/////////////////////////////////////////////////////////////////////////////

void DX9Renderer::cleanRenderTarget( const Color& bgColor )
{
   m_d3Device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColor, 1.0f, 0 );
}

/////////////////////////////////////////////////////////////////////////////

IDirect3DVertexBuffer9* DX9Renderer::createVertexBuffer(UINT length, 
                                                        DWORD usageFlags, 
                                                        DWORD fvf, 
                                                        D3DPOOL memoryPool)
{
   assert(length > 0);

   if (!m_hardwareTLOn) usageFlags |= D3DUSAGE_SOFTWAREPROCESSING;

   length = (length < 16) ? 16 : length;

   IDirect3DVertexBuffer9* vertexBuffer = NULL;
   HRESULT res = m_d3Device->CreateVertexBuffer(length,
                                                usageFlags,
                                                fvf,
                                                memoryPool,
                                                &vertexBuffer,
                                                NULL);

   if (FAILED(res))
   {
      std::string errorMsg = translateDxError( "Cannot create a vertex buffer", res );
      throw std::runtime_error( errorMsg );
   }

   return vertexBuffer;
}

/////////////////////////////////////////////////////////////////////////////

IDirect3DIndexBuffer9* DX9Renderer::createIndexBuffer(UINT length, 
                                                      DWORD usageFlags, 
                                                      D3DFORMAT format, 
                                                      D3DPOOL memoryPool)
{
   if (!m_hardwareTLOn) usageFlags |= D3DUSAGE_SOFTWAREPROCESSING;

   IDirect3DIndexBuffer9* indexBuffer = NULL;

   HRESULT res = m_d3Device->CreateIndexBuffer(length,
      usageFlags,
      format,
      memoryPool,
      &indexBuffer,
      NULL);

   if (FAILED(res))
   {
      throw std::logic_error(
         std::string("Cannot create an index buffer"));
   }

   return indexBuffer;
}

/////////////////////////////////////////////////////////////////////////////

const D3DFORMAT g_ColorTextureFormats[] = { D3DFMT_DXT5, D3DFMT_DXT3, D3DFMT_X8R8G8B8, D3DFMT_DXT1, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5, D3DFMT_UNKNOWN };
const D3DFORMAT g_DepthTextureFormats[] = { D3DFMT_D32, D3DFMT_D24S8, D3DFMT_D24X8, D3DFMT_D24X4S4, D3DFMT_D16, D3DFMT_D15S1, D3DFMT_UNKNOWN };
const D3DFORMAT g_LuminanceTextureFormats[] = { D3DFMT_G32R32F, D3DFMT_G16R16F, D3DFMT_UNKNOWN };
const D3DFORMAT g_HDRTextureFormats[] = { D3DFMT_A32B32G32R32F, D3DFMT_A16B16G16R16F, D3DFMT_UNKNOWN };

const D3DFORMAT* g_textureFormats[] =
{
   g_ColorTextureFormats,           // TU_COLOR
   g_DepthTextureFormats,           // TU_DEPTH
   g_LuminanceTextureFormats,       // TU_LUMINANCE 
   g_HDRTextureFormats,             // TU_HDR
};

void DX9Renderer::sampleOptimalTextureFormats()
{
   for ( UINT usage = 0; usage < TU_MAX; ++usage )
   {
      const D3DFORMAT* formats = g_textureFormats[ usage ];

      UINT i = 0;
      D3DFORMAT bestFormat = D3DFMT_UNKNOWN;
      for ( UINT i = 0; formats[i] != D3DFMT_UNKNOWN; ++i )
      {
         HRESULT res = m_d3d9.CheckDeviceFormat( m_settings.adapterOrdinal, 
            m_settings.deviceType, 
            m_settings.displayMode.Format,
            0, D3DRTYPE_TEXTURE, 
            formats[i] );

         if ( SUCCEEDED( res ) )
         {
            bestFormat = formats[i];
            break;
         }
      }

      ASSERT_MSG( bestFormat != D3DFMT_UNKNOWN, "No dedicated texture format found for this usage." );
      m_bestTextureFormats[usage] = bestFormat;
   }
}

/////////////////////////////////////////////////////////////////////////////
