#include "impl-DirectX\D3DApplicationManager.h"
#include <tchar.h>
#include "core\WindowBuilder.h"
#include "impl-DirectX\D3DRenderer.h"
#include "impl-DirectX\D3DResourceManager.h"
#include "impl-DirectX\D3DInitializer.h"
#include "core\Timer.h"
#include <cassert>
#include "core\dostream.h"


///////////////////////////////////////////////////////////////////////////////

D3DApplicationManager::D3DApplicationManager(HINSTANCE hInstance, 
                                             int nCmdShow,
                                             const std::string& programName)
      : m_hInstance(hInstance),
      m_programName(programName),
      m_timer(new CTimer()),
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

   m_hWnd = winBuilder.createWindowedModeWindow(m_hInstance, params);
   assert(m_hWnd != NULL);

   m_resourceManager = new D3DResourceManager("..\\Data", "..\\Data", m_hWnd, true);
   m_renderer = &(m_resourceManager->getRendererInstance());

   ShowWindow(m_hWnd, nCmdShow);
}

///////////////////////////////////////////////////////////////////////////////

D3DApplicationManager::~D3DApplicationManager()
{
   m_renderer = NULL;

   delete m_resourceManager;
   m_resourceManager = NULL;

   delete m_timer;
   m_timer = NULL;
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

Renderer& D3DApplicationManager::getRenderer() 
{
   return *m_renderer;
}

///////////////////////////////////////////////////////////////////////////////

ResourceManager& D3DApplicationManager::getResourceManager()
{
   return *m_resourceManager;
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
