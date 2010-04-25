#include "core\WindowBuilder.h"
#include "core\WindowMessagesProcessor.h"


///////////////////////////////////////////////////////////////////////////////

CWindowBuilder::CWindowBuilder(void)
{
}

///////////////////////////////////////////////////////////////////////////////

CWindowBuilder::~CWindowBuilder(void)
{
}

///////////////////////////////////////////////////////////////////////////////

HWND CWindowBuilder::createFullScreenWindow(HINSTANCE hInstance, 
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

bool CWindowBuilder::m_windowInitialized = false;

HWND CWindowBuilder::createWindowedModeWindow(HINSTANCE hInstance, 
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

LRESULT CALLBACK CWindowBuilder::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
