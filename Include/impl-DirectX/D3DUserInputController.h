#pragma once

#include "core-AppFlow\UserInputController.h"
#include "core\WindowMessagesProcessor.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class D3DUserInputController : public UserInputController,
                               public IWindowMessagesProcessor
{
private:   
   bool m_rightMouseButton;
   bool m_leftMouseButton;

public:
   D3DUserInputController();

protected:
   void onRelativeMouseMovement();
   void onAbsoluteMouseMovement();

   void checkUserInput(unsigned char* keyBuffer, Point& mousePos);

   void setMousePos(const Point& pos);

   bool wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

///////////////////////////////////////////////////////////////////////////////
