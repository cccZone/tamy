#include "dx9-Renderer\DX9Initializer.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core-Renderer\RenderingDevice.h"
#include <stdexcept>
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

DX9Initializer::DX9Initializer(IDirect3D9& d3d9)
      : m_d3d9(d3d9)
{
}

/////////////////////////////////////////////////////////////////////////////

DX9Renderer* DX9Initializer::createDisplay(DX9Settings& settings,  
                                           HWND hWnd, 
                                           ULONG Flags)
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

   D3DFORMAT optimalTextureFormat = findOptimalTextureFormat(settings);

   IDirect3DDevice9* d3Device = NULL;
   HRESULT deviceCreationResult = m_d3d9.CreateDevice(settings.adapterOrdinal, 
                                                      settings.deviceType, 
                                                      hWnd,  // focus window
                                                      creationFlags, 
                                                      &presentParams, 
                                                      &d3Device);
   if (FAILED(deviceCreationResult) || (d3Device == NULL))
   {
      std::string errMsg = "Renderer could not be created ";
      switch(deviceCreationResult)
      {
      case D3DERR_DEVICELOST: errMsg += "due to the device being lost"; break;
      case D3DERR_INVALIDCALL: errMsg += "due to invalid init params"; break;
      case D3DERR_NOTAVAILABLE: errMsg += "due to unavailable device"; break;
      case D3DERR_OUTOFVIDEOMEMORY: errMsg += "due to lack of video memory"; break;
      default: errMsg += "- unknown problem occured"; break;
      }
      throw std::runtime_error(errMsg);
   }
   else
   {
      return new DX9Renderer(d3Device, 
                             presentParams, 
                             settings.caps,
                             width,
                             height,
                             hardwareTLOn,
                             optimalTextureFormat);
   }
}

/////////////////////////////////////////////////////////////////////////////
const UINT TextureFormatsCount = 6;
const D3DFORMAT TextureFormats[6] = {D3DFMT_DXT5, D3DFMT_DXT3, D3DFMT_X8R8G8B8,
                                     D3DFMT_DXT1, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5};

D3DFORMAT DX9Initializer::findOptimalTextureFormat(DX9Settings& settings)
{
   for (UINT i = 0; i < TextureFormatsCount; ++i)
   {
      if (SUCCEEDED(m_d3d9.CheckDeviceFormat(settings.adapterOrdinal, 
                                             settings.deviceType, 
                                             settings.displayMode.Format,
                                             0, D3DRTYPE_TEXTURE, 
                                             TextureFormats[i])))
      {
         return TextureFormats[i];
      }
   }

   return D3DFMT_UNKNOWN;
}

/////////////////////////////////////////////////////////////////////////////
