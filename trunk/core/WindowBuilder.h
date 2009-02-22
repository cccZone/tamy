#pragma once

#include <windows.h>
#include <tchar.h>

#define MAX_LOADSTRING 255

class IWindowMessagesProcessor;

struct WindowParams
{
   char windowTitle[MAX_LOADSTRING];
   char windowClassName[MAX_LOADSTRING];
   UINT largeIconID;
   UINT smallIconID;
   UINT menuID;
   UINT width;
   UINT height;
   IWindowMessagesProcessor* ptrMsgProc;

   WindowParams()
      : largeIconID(-1),
      smallIconID(-1),
      menuID(-1),
      width(800),
      height(600),
      ptrMsgProc(NULL)
   {
      windowTitle[0] = 0;
      windowClassName[0] = 0;
   }
};

class CWindowBuilder
{
public:
   CWindowBuilder(void);
   ~CWindowBuilder(void);

   HWND createFullScreenWindow(HINSTANCE hInstance, 
                               const WindowParams& windowParams) const;

   HWND createWindowedModeWindow(HINSTANCE hInstance, 
                                 const WindowParams& windowParams) const;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

