#include "dx9-Renderer\DX9Initializer.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "dx9-Renderer\DXErrorParser.h"
#include "core-Renderer\RenderingDevice.h"
#include "core\Assert.h"
#include <string>


/////////////////////////////////////////////////////////////////////////////

DX9Settings::DX9Settings(RenderingDevice& device)
{
   adapterOrdinal = device.adapterOrdinal;
   backBufferFormat = device.backBufferFormat;
   depthStencilFormat = device.depthStencilFormats[0];
   deviceType = device.deviceType;
   displayMode = device.displayMode;
   multisampleType = device.multisampleTypes[0];
   multisampleQuality = device.multisampleQualities[0];
   presentInterval = device.presentIntervals[0];
   vertexProcessingType = device.vertexProcessingType;
   windowed = device.windowed;
   caps = device.caps;
}

/////////////////////////////////////////////////////////////////////////////

DX9Initializer::DX9Initializer( IDirect3D9& d3d9 )
: m_d3d9( d3d9 )
{
}

/////////////////////////////////////////////////////////////////////////////

IDirect3DDevice9* DX9Initializer::createNullDevice()
{
   D3DDISPLAYMODE mode;
   m_d3d9.GetAdapterDisplayMode(0, &mode );

   D3DPRESENT_PARAMETERS pp;
   ZeroMemory( &pp, sizeof(D3DPRESENT_PARAMETERS) ); 
   pp.BackBufferWidth  = 1;
   pp.BackBufferHeight = 1;
   pp.BackBufferFormat = mode.Format;
   pp.BackBufferCount  = 1;
   pp.SwapEffect       = D3DSWAPEFFECT_COPY;
   pp.Windowed         = TRUE;

   IDirect3DDevice9* d3Device;
   HRESULT deviceCreationResult = m_d3d9.CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, GetConsoleWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &d3Device );
   if ( FAILED( deviceCreationResult ) || !d3Device )
   {
      std::string errMsg = translateDxError( "Renderer could not be created: ", deviceCreationResult );
      ASSERT_MSG( false, errMsg.c_str() );
      return NULL;
   }

   return d3Device;
}

/////////////////////////////////////////////////////////////////////////////

DX9Renderer* DX9Initializer::createDisplay( DX9Settings& settings, HWND hWnd, ULONG Flags )
{
   D3DPRESENT_PARAMETERS presentParams;
   ZeroMemory(&presentParams, sizeof(D3DPRESENT_PARAMETERS));

   presentParams.EnableAutoDepthStencil = true;
   presentParams.AutoDepthStencilFormat = settings.depthStencilFormat;
   presentParams.hDeviceWindow = hWnd;
   presentParams.Windowed = settings.windowed;
   presentParams.PresentationInterval = settings.presentInterval;
   presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
   presentParams.BackBufferCount = 0;
   presentParams.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
   presentParams.BackBufferFormat = settings.backBufferFormat;

   UINT width, height;
   if (settings.windowed)
   {
      RECT rc;
      ::GetClientRect(hWnd, &rc);
      width = rc.right - rc.left;
      height = rc.bottom - rc.top;
   }
   else
   {
      width = settings.displayMode.Width;
      height = settings.displayMode.Height;
   }

   presentParams.BackBufferWidth = width;
   presentParams.BackBufferHeight = height;

   ULONG creationFlags = 0;
   bool hardwareTLOn;
   switch(settings.vertexProcessingType)
   {
   case SOFTWARE_VP:
      {
         creationFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
         hardwareTLOn = false;
         break;
      }

   case MIXED_VP:
      {
         creationFlags |= D3DCREATE_MIXED_VERTEXPROCESSING;
         hardwareTLOn = false;
         break;
      }

   case HARDWARE_VP:
      {
         creationFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
         hardwareTLOn = true;
         break;
      }

   case PURE_HARDWARE_VP:
      {
         creationFlags |= D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING;
         hardwareTLOn = true;
         break;
      }
   }

   IDirect3DDevice9* d3Device = NULL;
   HRESULT deviceCreationResult = m_d3d9.CreateDevice(settings.adapterOrdinal, 
                                                      settings.deviceType, 
                                                      hWnd,  // focus window
                                                      creationFlags, 
                                                      &presentParams, 
                                                      &d3Device);
   if (FAILED(deviceCreationResult) || (d3Device == NULL))
   {
      std::string errMsg = translateDxError( "Renderer could not be created: ", deviceCreationResult );
      ASSERT_MSG( false, errMsg.c_str() );
      return NULL;
   }
   else
   {
      return new DX9Renderer(m_d3d9, d3Device, settings, presentParams, width, height, hardwareTLOn);
   }
}

/////////////////////////////////////////////////////////////////////////////
