#include "core-Renderer\RenderDevicesEnumerator.h"
#include "core-Renderer\RenderingDevice.h"
#include "core\WindowBuilder.h"
#include <d3d9.h>
#include <fstream>


///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   std::fstream deviceDescFile("devicesTested.log" , std::ios_base::out);

   // create a window
   HWND hWndMock;
   CWindowBuilder winBuilder;
   WindowParams winParams;
   strcpy_s(winParams.windowTitle, "Graph Devices Enumeration Demo");
   strcpy_s(winParams.windowClassName, "Graph Devices Enumeration Demo Class");
   winParams.ptrMsgProc = NULL;
   winParams.width = 320;
   winParams.height = 200;
   hWndMock = winBuilder.createWindowedModeWindow(hInstance, nCmdShow, winParams);
   

   // initialize graphics system
   IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

   // create the database of rendering devices and make sure it's not empty
   RenderDevicesEnumerator enumerator(*d3d9);

   std::vector<RenderingDevice*> devicesDB;
   enumerator.getDevices(devicesDB);
   unsigned int devicesCount = devicesDB.size();

   if (devicesCount == 0)
   {
      deviceDescFile << "No graphics devices found" << std::endl;
      return -1;
   }

   // show all the devices found
   deviceDescFile << devicesCount << " device modes found" << std::endl 
                  << std::endl << "-------------- TEST RESULTS ---------------" << std::endl << std::endl;


   // instantiate each one and check if it can be created
   for (unsigned int i = 0; i < devicesCount; ++i)
   {
      RenderingDevice& settings = *(devicesDB[i]);

      D3DPRESENT_PARAMETERS presentParams;
      ZeroMemory(&presentParams, sizeof(D3DPRESENT_PARAMETERS));

      presentParams.EnableAutoDepthStencil = true;
      presentParams.AutoDepthStencilFormat = settings.depthStencilFormats[0];
      presentParams.hDeviceWindow = hWndMock;
      presentParams.Windowed = settings.windowed;
      presentParams.PresentationInterval = settings.presentIntervals[0];
      presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
      presentParams.BackBufferCount = 0;
      presentParams.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
      presentParams.BackBufferFormat = settings.backBufferFormat;
      presentParams.BackBufferWidth = settings.displayMode.Width;
      presentParams.BackBufferHeight = settings.displayMode.Height;

      ULONG creationFlags = 0;
      switch(settings.vertexProcessingType)
      {
      case SOFTWARE_VP:
         {
            creationFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
            break;
         }

      case MIXED_VP:
         {
            creationFlags |= D3DCREATE_MIXED_VERTEXPROCESSING;
            break;
         }

      case HARDWARE_VP:
         {
            creationFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
            break;
         }

      case PURE_HARDWARE_VP:
         {
            creationFlags |= D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING;
            break;
         }
      }

      deviceDescFile << settings.getDesc().c_str() << std::endl;

      IDirect3DDevice9* d3Device = NULL;
      HRESULT deviceCreationResult = d3d9->CreateDevice(settings.adapterOrdinal, 
                                                        settings.deviceType, 
                                                        hWndMock,
                                                        creationFlags, 
                                                        &presentParams, 
                                                        &d3Device);
      if (FAILED(deviceCreationResult) || (d3Device == NULL))
      {
         char tmpStr[64];
         sprintf_s(tmpStr, 64, "Renderer %d could not be created ", i);
         std::string errMsg = tmpStr;
         switch(deviceCreationResult)
         {
         case D3DERR_DEVICELOST: errMsg += "due to the device being lost"; break;
         case D3DERR_INVALIDCALL: errMsg += "due to invalid init params"; break;
         case D3DERR_NOTAVAILABLE: errMsg += "due to unavailable device"; break;
         case D3DERR_OUTOFVIDEOMEMORY: errMsg += "due to lack of video memory"; break;
         default: errMsg += "- unknown problem occured"; break;
         }

         deviceDescFile << "[FAILURE] - " << errMsg.c_str() << std::endl << std::endl;
      }
      else
      {
         deviceDescFile << "[SUCCESS]" << std::endl << std::endl;
         d3Device->Release();
      }
   }


   // cleanup
   for (unsigned int i = 0; i < devicesCount; ++i)
   {
      delete devicesDB[i];
   }
   d3d9->Release();
   DestroyWindow(hWndMock);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
