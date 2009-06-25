#include "impl-DirectX\D3DApplicationManager.h"
#include <tchar.h>
#include "core\WindowBuilder.h"
#include "impl-DirectX\D3DRenderer.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "impl-DirectX\D3DInitializer.h"
#include "core\Timer.h"
#include <cassert>
#include "core\dostream.h"
#include "core-ResourceManagement\MaterialsParser.h"
#include "core-ResourceManagement\GraphicalEntityLoaderFactory.h"

#include "impl-DirectX\D3DInitializer.h"
#include "impl-DirectX\D3DRenderer.h"
#include "impl-DirectX\D3DGraphicalEntity.h"
#include "impl-DirectX\D3DSkinnedGraphicalEntity.h"
#include "impl-DirectX\D3DLight.h"
#include "impl-DirectX\D3DLightReflectingProperties.h"
#include "impl-DirectX\D3DTexture.h"
#include "impl-DirectX\D3DEmptyTexture.h"
#include "impl-DirectX\D3DSkyBox.h"
#include "impl-DirectX\D3DMaterial.h"
#include "impl-DirectX\XFileGraphicalEntityLoader.h"
#include "impl-DirectX\D3DColorOperationImplementation.h"
#include "impl-DirectX\D3DAlphaOperationImplementation.h"
#include "impl-DirectX\D3DParticleSystem.h"
#include <stdexcept>
#include <string>
#include <cassert>

#include "core-Renderer\BackgroundPass.h"
#include "core-Renderer\DrawingPass.h"

#include "impl-DirectX\OpenALSoundSystem.h"
#include "impl-DirectX\SoundInitializer.h"
#include "impl-DirectX\SoundDeviceInfo.h"
#include "impl-DirectX\OALSoundDevice.h"
#include "impl-DirectX\OALSoundListener.h"
#include "impl-DirectX\OALSound3D.h"
#include "core-Sound\SoundRenderer.h"


///////////////////////////////////////////////////////////////////////////////

D3DApplicationManager::D3DApplicationManager(const std::string& texturesDir,
                                             const std::string& fontsDir,
                                             const std::string& meshesDir,
                                             HINSTANCE hInstance, 
                                             int nCmdShow,
                                             const std::string& programName)
      : ApplicationManager(texturesDir, fontsDir, meshesDir),
      m_hInstance(hInstance),
      m_programName(programName),
      m_timer(new CTimer()),
      m_d3d9(NULL),
      m_d3dInitializer(NULL),
      m_renderer(NULL),
      m_lastFrameRate(0),
      m_rightMouseButton(false),
      m_leftMouseButton(false),
      m_relativeMouseMovement(false)
{
   m_oldCursorPos.x = 0;
   m_oldCursorPos.y = 0;

   CWindowBuilder winBuilder;

   WindowParams params;
   strcpy_s(params.windowTitle, programName.c_str());
   strcpy_s(params.windowClassName, (programName + std::string("Class")).c_str());
   params.ptrMsgProc = this;
   params.width = 1024;
   params.height = 768;

   m_hWnd = winBuilder.createWindowedModeWindow(m_hInstance, params);
   assert(m_hWnd != NULL);

   // prepare resMgr for work
   resMgr().setShared<MaterialsParser>(new MaterialsParser(resMgr()));

   m_renderer = createRenderer(true);
   resMgr().setShared<Renderer>(m_renderer);
   resMgr().setShared<D3DRenderer>(*m_renderer);
   resMgr().setShared<IDirect3DDevice9>(m_renderer->getD3Device());

   resMgr().setShared<D3DColorOperationImplementation>(new D3DColorOperationImplementation(m_renderer->getD3Device()));
   resMgr().setShared<D3DAlphaOperationImplementation>(new D3DAlphaOperationImplementation(m_renderer->getD3Device()));

   OpenALSoundSystem* soundSystem = new OpenALSoundSystem();

   SoundInitializer soundInit(*soundSystem);
   SoundDeviceInfo idealDevice(OALV_UNKNOWN, 8);
   const SoundDeviceInfo& bestDevice = soundInit.findBest(idealDevice);
   OALSoundDevice* soundDevice = soundSystem->createDevice(bestDevice);
   resMgr().setShared<SoundRenderer>(new SoundRenderer(*soundDevice));
   resMgr().setShared<SoundDevice>(soundDevice);
   resMgr().setShared<OpenALSoundSystem>(soundSystem);

   registerFactories<IDirect3D9>();

   // register additional file readers
   resMgr().resource<GraphicalEntityLoader>().add(new XFileGraphicalEntityLoader(m_renderer->getD3Device()));


   // show the application window
   ShowWindow(m_hWnd, nCmdShow);
}

///////////////////////////////////////////////////////////////////////////////

D3DApplicationManager::~D3DApplicationManager()
{
   delete m_timer;
   m_timer = NULL;

   delete m_d3dInitializer;
   m_d3dInitializer = NULL;

   m_d3d9->Release();
   m_d3d9 = NULL;

   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

D3DRenderer* D3DApplicationManager::createRenderer(bool windowed)
{
   m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
   if (m_d3d9 == NULL)
   {
      throw std::logic_error(std::string("Cannot initialize DirectX library"));
   }

   m_d3dInitializer = new D3DInitializer(*m_d3d9, m_hWnd, *this);
   if (m_d3dInitializer == NULL)
   {
      m_d3d9->Release();
      m_d3d9 = NULL;
      throw std::logic_error(std::string("Failed to create a Direct3D initializer"));
   }

   D3DSettings d3DSettings;
   if (windowed)
   {
      d3DSettings = m_d3dInitializer->findBestWindowedMode();
   }
   else
   {
      D3DDISPLAYMODE matchMode;
      matchMode.Width = 1024;
      matchMode.Width = 768;
      matchMode.Format = D3DFMT_X8R8G8B8;
      d3DSettings = m_d3dInitializer->findBestFullscreenMode(matchMode);
   }
   D3DRenderer* renderer = m_d3dInitializer->createDisplay(d3DSettings, m_hWnd);
   renderer->addPass(new BackgroundPass());
   renderer->addPass(new DrawingPass());

   return renderer;
}

///////////////////////////////////////////////////////////////////////////////

bool D3DApplicationManager::checkDeviceCaps(const D3DCAPS9& caps)
{
   if(!(caps.FVFCaps & D3DFVFCAPS_PSIZE)) {return false;}
   if(caps.MaxPointSize <= 1.0f) {return false;}

   return true;
}

///////////////////////////////////////////////////////////////////////////////

ApplicationManager::ProcessingCode D3DApplicationManager::onStep()
{
    // process system messages
   MSG msg;
   if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
   {
      if (msg.message == WM_QUIT) {return APC_EXIT;}

      TranslateMessage(&msg);
      DispatchMessage (&msg);
      return APC_SYSTEM;
   } 
   else
   {
      advanceApplicationState();

      return APC_APPLICATION;
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DApplicationManager::advanceApplicationState()
{
   m_timer->tick();

   static char titleBuffer[255];  
   if (m_lastFrameRate != m_timer->getFrameRate())
   {
      m_lastFrameRate = m_timer->getFrameRate();
      sprintf_s(titleBuffer, ("%s : %ld FPS"), m_programName.c_str(), m_lastFrameRate);
      SetWindowText(m_hWnd, titleBuffer);
   }
}

///////////////////////////////////////////////////////////////////////////////

LRESULT D3DApplicationManager::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
   case WM_SIZE: // fallthrough
   case WM_MOVE:
      {
         WINDOWINFO info;
         GetWindowInfo(hWnd, &info);

         if (m_renderer)
         {
            m_renderer->resizeViewport(info.rcClient.right - info.rcClient.left, 
                                       info.rcClient.bottom - info.rcClient.top, 
                                       info.rcClient.left, 
                                       info.rcClient.top,
                                       info.rcClient.right, 
                                       info.rcClient.bottom);
         }

         break;
      }

   case WM_RBUTTONDOWN:
      {  
         m_rightMouseButton = true;  
         break;
      }

   case WM_LBUTTONDOWN:
      {
         m_leftMouseButton = true;
         break;
      }

   case WM_RBUTTONUP:
      {
         m_rightMouseButton = false;
         break;
      }

   case WM_LBUTTONUP:
      {
         m_leftMouseButton = false;
         break;
      }

   case WM_DESTROY:
      {
         PostQuitMessage(0);
         break;
      }

   default:
      {
         return DefWindowProc(hWnd, message, wParam, lParam);
      }
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////

float D3DApplicationManager::getTimeElapsed()
{
   return m_timer->getTimeElapsed();
}

///////////////////////////////////////////////////////////////////////////////

void D3DApplicationManager::checkUserInput(unsigned char* keysBuffer, Point& mousePos)
{
   if (!GetKeyboardState(keysBuffer)) return;

   if (m_rightMouseButton) keysBuffer[VK_RBUTTON] = 0xF0;
   if (m_leftMouseButton) keysBuffer[VK_LBUTTON] = 0xF0;

   POINT cursorPos;
   GetCursorPos(&cursorPos);
   if (m_relativeMouseMovement)
   {
      mousePos.x = cursorPos.x - m_oldCursorPos.x;
      mousePos.y = cursorPos.y - m_oldCursorPos.y;
      SetCursorPos(m_oldCursorPos.x, m_oldCursorPos.y);
   }
   else
   {
      mousePos.x = cursorPos.x;
      mousePos.y = cursorPos.y;
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DApplicationManager::switchMouseMovementMode(bool relative)
{
   m_relativeMouseMovement = relative;

   if (relative)
   {
      SetCapture(m_hWnd);
      SetCursor(NULL);
      GetCursorPos(&m_oldCursorPos);
   }
   else
   {
      ReleaseCapture();
      SetCursor(NULL);
   }
}

///////////////////////////////////////////////////////////////////////////////
