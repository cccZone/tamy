#include "win-AppFlow\WinWindowBuilder.h"
#include <tchar.h>
#include "core-AppFlow\IWindowMessagesProcessor.h"


///////////////////////////////////////////////////////////////////////////////

bool WinWindowBuilder::m_windowInitialized = false;

///////////////////////////////////////////////////////////////////////////////

WinWindowBuilder::WinWindowBuilder(HINSTANCE hInstance,
                                   int nCmdShow,
                                   const std::string& appName,
                                   unsigned int winWidth,
                                   unsigned int winHeight,
                                   bool windowed,
                                   HICON icon)
: m_hInstance(hInstance)
, m_nCmdShow(nCmdShow)
, m_appName(appName)
, m_winWidth(winWidth)
, m_winHeight(winHeight)
, m_windowed(windowed)
, m_icon(icon)
{
}

///////////////////////////////////////////////////////////////////////////////

HWND WinWindowBuilder::create(IWindowMessagesProcessor& winMsgProcessor)
{
   WindowParams params;
   strcpy_s(params.windowTitle, m_appName.c_str());
   strcpy_s(params.windowClassName, (m_appName + "Class").c_str());
   params.ptrMsgProc = &winMsgProcessor;
   params.width = m_winWidth;
   params.height = m_winHeight;
   params.smallIcon = m_icon;
   params.largeIcon = m_icon;

   HWND hWnd;
   if (m_windowed)
   {
      hWnd = createWindowedModeWindow(m_hInstance, m_nCmdShow, params);
   }
   else
   {
      hWnd = createFullScreenWindow(m_hInstance, m_nCmdShow, params);
   }

   return hWnd;
}

///////////////////////////////////////////////////////////////////////////////

HWND WinWindowBuilder::createFullScreenWindow(HINSTANCE hInstance, 
                                              int nCmdShow,
                                              const WindowParams& windowParams) const
{
   WNDCLASSEX wcex;
   wcex.cbSize = sizeof(WNDCLASSEX);

   wcex.style			   = CS_BYTEALIGNCLIENT;
   wcex.lpfnWndProc	   = WndProc;
   wcex.cbClsExtra		= 0;
   wcex.cbWndExtra		= 0;
   wcex.hInstance		   = hInstance;
   wcex.hIcon			   = NULL;
   wcex.hCursor		   = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName	   = NULL;
   wcex.lpszClassName	= windowParams.windowClassName;
   wcex.hIconSm		   = NULL;
   RegisterClassEx(&wcex);

   m_windowInitialized = false;
   HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, windowParams.windowClassName, windowParams.windowTitle, 0,
      0, 0, windowParams.width, windowParams.height, NULL, NULL, hInstance, windowParams.ptrMsgProc);

   ShowWindow(hWnd, nCmdShow);

   MSG msg;
   while(m_windowInitialized == false)
   {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
      {
         TranslateMessage(&msg);
         DispatchMessage (&msg);
      } 
   }

   return hWnd;
}

///////////////////////////////////////////////////////////////////////////////

HWND WinWindowBuilder::createWindowedModeWindow(HINSTANCE hInstance, 
                                                int nCmdShow,
                                                const WindowParams& windowParams) const
{
   WNDCLASSEX wcex;
   wcex.cbSize = sizeof(WNDCLASSEX);

   wcex.style			   = CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc	   = WndProc;
   wcex.cbClsExtra		= 0;
   wcex.cbWndExtra		= 0;
   wcex.hInstance		   = hInstance;
   wcex.hIcon			   = windowParams.largeIcon;
   wcex.hCursor		   = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName	   = (windowParams.menuID >= 0) ? MAKEINTRESOURCE(windowParams.menuID) : NULL;
   wcex.lpszClassName	= windowParams.windowClassName;
   wcex.hIconSm		   = windowParams.smallIcon;
   RegisterClassEx(&wcex);

   m_windowInitialized = false;
   HWND hWnd = CreateWindowEx(0, windowParams.windowClassName, windowParams.windowTitle, 
      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
      0, 0, windowParams.width, windowParams.height, 
      NULL, NULL, hInstance, windowParams.ptrMsgProc);
   ShowWindow(hWnd, nCmdShow);

   MSG msg;
   while(m_windowInitialized == false)
   {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
      {
         TranslateMessage(&msg);
         DispatchMessage (&msg);
      } 
   }

   return hWnd;
}

///////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WinWindowBuilder::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   // If this is a create message, trap the 'this' pointer passed in and store it within the window.
   if (message == WM_CREATE) 
   {
      SetWindowLong(hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);
      m_windowInitialized = true;
   }

   IWindowMessagesProcessor *destination = (IWindowMessagesProcessor*)GetWindowLong(hWnd, GWL_USERDATA);
   bool wasMessageProcessed = false;
   if (destination) 
   {
      wasMessageProcessed = destination->wndProc(hWnd, message, wParam, lParam);
   }

   if (wasMessageProcessed == false)
   {
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
   else
   {
      return 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
