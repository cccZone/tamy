#include "impl-DirectX\D3DApplicationManager.h"
#include <tchar.h>
#include "core\WindowBuilder.h"
#include "core\Timer.h"
#include "core-Renderer\Renderer.h"
#include <cassert>
#include "core\dostream.h"
#include "impl-DirectX\Tamy.h"


///////////////////////////////////////////////////////////////////////////////

D3DApplicationManager::D3DApplicationManager(HINSTANCE hInstance, 
                                             int nCmdShow,
                                             const std::string& programName,
                                             Tamy& tamy)
      : m_hInstance(hInstance),
      m_programName(programName),
      m_tamy(tamy),
      m_timer(new CTimer()),
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

   m_tamy.initialize(m_hWnd);
   m_renderer = &(m_tamy.renderer());

   // show the application window
   ShowWindow(m_hWnd, nCmdShow);
}

///////////////////////////////////////////////////////////////////////////////

D3DApplicationManager::~D3DApplicationManager()
{
   delete m_timer;
   m_timer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

float D3DApplicationManager::getTimeElapsed()
{
   return m_timer->getTimeElapsed();
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
         if (m_renderer == NULL) {break;}

         WINDOWINFO info;
         GetWindowInfo(hWnd, &info);

         m_renderer->resizeViewport(info.rcClient.right - info.rcClient.left, 
                                    info.rcClient.bottom - info.rcClient.top, 
                                    info.rcClient.left, 
                                    info.rcClient.top,
                                    info.rcClient.right, 
                                    info.rcClient.bottom);

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
