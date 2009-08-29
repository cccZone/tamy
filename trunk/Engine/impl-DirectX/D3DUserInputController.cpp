#include "impl-DirectX\D3DUserInputController.h"


///////////////////////////////////////////////////////////////////////////////

D3DUserInputController::D3DUserInputController()
      : m_rightMouseButton(false),
      m_leftMouseButton(false)
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DUserInputController::onRelativeMouseMovement()
{
   SetCursor(NULL);
}

///////////////////////////////////////////////////////////////////////////////

void D3DUserInputController::onAbsoluteMouseMovement()
{
   SetCursor(NULL);
}

///////////////////////////////////////////////////////////////////////////////

void D3DUserInputController::checkUserInput(unsigned char* keysBuffer, Point& mousePos)
{
   if (!GetKeyboardState(keysBuffer)) return;

   if (m_rightMouseButton) keysBuffer[VK_RBUTTON] = 0xF0;
   if (m_leftMouseButton) keysBuffer[VK_LBUTTON] = 0xF0;

   POINT cursorPos;
   GetCursorPos(&cursorPos);
   mousePos.x = cursorPos.x;
   mousePos.y = cursorPos.y;
}

///////////////////////////////////////////////////////////////////////////////

void D3DUserInputController::setMousePos(const Point& pos)
{
   SetCursorPos(pos.x, pos.y);
}

///////////////////////////////////////////////////////////////////////////////

bool D3DUserInputController::wndProc(HWND hWnd, 
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
