#include "WindowBuilder.h"
#include "WindowMessagesProcessor.h"


CWindowBuilder::CWindowBuilder(void)
{
}

CWindowBuilder::~CWindowBuilder(void)
{
}

HWND CWindowBuilder::createFullScreenWindow(HINSTANCE hInstance, 
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

   HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, windowParams.windowClassName, windowParams.windowTitle, 0,
      0, 0, windowParams.width, windowParams.height, NULL, NULL, hInstance, windowParams.ptrMsgProc);

   return hWnd;
}

HWND CWindowBuilder::createWindowedModeWindow(HINSTANCE hInstance, 
                                              const WindowParams& windowParams) const
{
   WNDCLASSEX wcex;
   wcex.cbSize = sizeof(WNDCLASSEX);

   wcex.style			   = CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc	   = WndProc;
   wcex.cbClsExtra		= 0;
   wcex.cbWndExtra		= 0;
   wcex.hInstance		   = hInstance;
   wcex.hIcon			   = (windowParams.largeIconID >= 0) ? LoadIcon(hInstance, MAKEINTRESOURCE(windowParams.largeIconID)) : NULL;
   wcex.hCursor		   = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName	   = (windowParams.menuID >= 0) ? MAKEINTRESOURCE(windowParams.menuID) : NULL;
   wcex.lpszClassName	= windowParams.windowClassName;
   wcex.hIconSm		   = (windowParams.smallIconID >= 0) ? LoadIcon(hInstance, MAKEINTRESOURCE(windowParams.smallIconID)) : NULL;
   RegisterClassEx(&wcex);

   HWND hWnd = CreateWindowEx(0, windowParams.windowClassName, windowParams.windowTitle, WS_OVERLAPPEDWINDOW,
      0, 0, windowParams.width, windowParams.height, NULL, NULL, hInstance, windowParams.ptrMsgProc);

   return hWnd;
}

LRESULT CALLBACK CWindowBuilder::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   // If this is a create message, trap the 'this' pointer passed in and store it within the window.
   if (message == WM_CREATE) 
   {
      SetWindowLong(hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);
   }

   IWindowMessagesProcessor *destination = (IWindowMessagesProcessor*)GetWindowLong(hWnd, GWL_USERDATA);
   if (destination) 
   {
      return destination->wndProc(hWnd, message, wParam, lParam);
   }
   else
   {
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
}
