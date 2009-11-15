#include "tamy\DefaultWindowBuilder.h"
#include <tchar.h>
#include "core\WindowBuilder.h"


///////////////////////////////////////////////////////////////////////////////

DefaultWindowBuilder::DefaultWindowBuilder(HINSTANCE hInstance,
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

HWND DefaultWindowBuilder::create(IWindowMessagesProcessor& winMsgProcessor)
{
   CWindowBuilder winBuilder;
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
      hWnd = winBuilder.createWindowedModeWindow(m_hInstance, m_nCmdShow, params);
   }
   else
   {
      hWnd = winBuilder.createFullScreenWindow(m_hInstance, m_nCmdShow, params);
   }

   return hWnd;
}

///////////////////////////////////////////////////////////////////////////////
