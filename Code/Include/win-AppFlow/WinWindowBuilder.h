#pragma once

/// @file   win-AppFlow\WinWindowBuilder.h
/// @brief  creates a single window the whole area of which 
///         is dedicated to tamy renderer

#include "core-AppFlow\WindowBuilder.h"
#include <string>
#include <windows.h>
#include <tchar.h>


///////////////////////////////////////////////////////////////////////////////

#define MAX_LOADSTRING 255

///////////////////////////////////////////////////////////////////////////////

/**
 * This window builder implementation is dedicated to creating a rendering
 * widget on a Windows system.
 */
class WinWindowBuilder : public WindowBuilder
{
private:
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

private:
   static bool m_windowInitialized;

   HINSTANCE m_hInstance;
   int m_nCmdShow;
   std::string m_appName;
   unsigned int m_winWidth;
   unsigned int m_winHeight;
   bool m_windowed;
   HICON m_icon;

public:
   /**
    * Constructor.
    */
   WinWindowBuilder(HINSTANCE hInstance,
                    int nCmdShow,
                    const std::string& appName,
                    unsigned int winWidth,
                    unsigned int winHeight,
                    bool windowed,
                    HICON icon);

   HWND create(IWindowMessagesProcessor& winMsgProcessor);

private:
   HWND createFullScreenWindow(HINSTANCE hInstance, 
      int nCmdShow,
      const WindowParams& windowParams) const;

   HWND createWindowedModeWindow(HINSTANCE hInstance, 
      int nCmdShow,
      const WindowParams& windowParams) const;

   static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

///////////////////////////////////////////////////////////////////////////////
