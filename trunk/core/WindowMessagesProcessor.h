#pragma once

#include <windows.h>


class IWindowMessagesProcessor
{
public:
   virtual ~IWindowMessagesProcessor(void) {}

   virtual LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};
