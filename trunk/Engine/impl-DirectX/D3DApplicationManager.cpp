#include "impl-DirectX\D3DApplicationManager.h"
#include "core\Timer.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

D3DApplicationManager::D3DApplicationManager(const std::string& programName)
      : m_programName(programName),
      m_timer(new CTimer()),
      m_lastFrameRate(0)
{
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
      m_timer->tick();

      return APC_APPLICATION;
   }
}

///////////////////////////////////////////////////////////////////////////////

bool D3DApplicationManager::wndProc(HWND hWnd, 
                                    UINT message, 
                                    WPARAM wParam, 
                                    LPARAM lParam)
{
   switch (message)
   {
   case WM_DESTROY:
      {
         PostQuitMessage(0);
         break;
      }

   default:
      {
         return false;
      }
   }
   return true;
}

///////////////////////////////////////////////////////////////////////////////
