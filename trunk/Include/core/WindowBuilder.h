#pragma once

#include <windows.h>
#include <tchar.h>


///////////////////////////////////////////////////////////////////////////////

#define MAX_LOADSTRING 255

///////////////////////////////////////////////////////////////////////////////

class IWindowMessagesProcessor;

///////////////////////////////////////////////////////////////////////////////

struct WindowParams
{
   char windowTitle[MAX_LOADSTRING];
   char windowClassName[MAX_LOADSTRING];
   HICON largeIcon;
   HICON smallIcon;
   UINT menuID;
   UINT width;
   UINT height;
   IWindowMessagesProcessor* ptrMsgProc;

   WindowParams()
      : largeIcon(NULL),
      smallIcon(NULL),
      menuID(-1),
      width(800),
      height(600),
      ptrMsgProc(NULL)
   {
      windowTitle[0] = 0;
      windowClassName[0] = 0;
   }
};

///////////////////////////////////////////////////////////////////////////////

class CWindowBuilder
{
private:
   static bool m_windowInitialized;
public:
   CWindowBuilder(void);
   ~CWindowBuilder(void);

   HWND createFullScreenWindow(HINSTANCE hInstance, 
                               int nCmdShow,
                               const WindowParams& windowParams) const;

   HWND createWindowedModeWindow(HINSTANCE hInstance, 
                                 int nCmdShow,
                                 const WindowParams& windowParams) const;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

///////////////////////////////////////////////////////////////////////////////
