/// @file   win-AppFlow/WinUserInputController.h
/// @brief  Windows user input system
#pragma once

#include "core-AppFlow\UserInputController.h"
#include "win-AppFlow\IWindowMessagesProcessor.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * Windows user input system.
 */
class WinUserInputController : public UserInputController,
                               public IWindowMessagesProcessor
{
private:   
   bool m_rightMouseButton;
   bool m_leftMouseButton;

public:
   WinUserInputController();

protected:
   void onRelativeMouseMovement();
   void onAbsoluteMouseMovement();

   void checkUserInput(unsigned char* keyBuffer, Point& mousePos);

   void setMousePos(const Point& pos);

   bool wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

///////////////////////////////////////////////////////////////////////////////
