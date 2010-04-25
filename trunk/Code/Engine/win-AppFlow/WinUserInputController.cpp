#include "win-AppFlow\WinUserInputController.h"


///////////////////////////////////////////////////////////////////////////////

WinUserInputController::WinUserInputController()
      : m_rightMouseButton(false),
      m_leftMouseButton(false)
{
}

///////////////////////////////////////////////////////////////////////////////

void WinUserInputController::onRelativeMouseMovement()
{
   SetCursor(NULL);
}

///////////////////////////////////////////////////////////////////////////////

void WinUserInputController::onAbsoluteMouseMovement()
{
   SetCursor(NULL);
}

///////////////////////////////////////////////////////////////////////////////

void WinUserInputController::checkUserInput(unsigned char* keysBuffer, Point& mousePos)
{
   if (!GetKeyboardState(keysBuffer)) return;

   setKey(keysBuffer, VK_RBUTTON, m_rightMouseButton);
   setKey(keysBuffer, VK_RBUTTON, m_leftMouseButton);

   POINT cursorPos;
   GetCursorPos(&cursorPos);
   mousePos.x = cursorPos.x;
   mousePos.y = cursorPos.y;
}

///////////////////////////////////////////////////////////////////////////////

void WinUserInputController::setMousePos(const Point& pos)
{
   SetCursorPos(pos.x, pos.y);
}

///////////////////////////////////////////////////////////////////////////////

bool WinUserInputController::wndProc(HWND hWnd, 
                                     UINT message, 
                                     WPARAM wParam, 
                                     LPARAM lParam)
{
   switch (message)
   {
   case WM_RBUTTONDOWN:
      {  
         SetCapture(hWnd);
         m_rightMouseButton = true;  
         break;
      }

   case WM_LBUTTONDOWN:
      {
         SetCapture(hWnd);
         m_leftMouseButton = true;
         break;
      }

   case WM_RBUTTONUP:
      {
         ReleaseCapture();
         m_rightMouseButton = false;
         break;
      }

   case WM_LBUTTONUP:
      {
         ReleaseCapture();
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
         return false;
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////
