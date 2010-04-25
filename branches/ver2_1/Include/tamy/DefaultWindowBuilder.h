#pragma once

/// @file   tamy\DefaultWindowBuilder.h
/// @brief  creates a single window the whole area of which 
///         is dedicated to tamy renderer

#include "tamy\TamyWindowBuilder.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class DefaultWindowBuilder : public TamyWindowBuilder
{
private:
   HINSTANCE m_hInstance;
   int m_nCmdShow;
   std::string m_appName;
   unsigned int m_winWidth;
   unsigned int m_winHeight;
   bool m_windowed;
   HICON m_icon;

public:
   /**
    * Construcor.
    */
   DefaultWindowBuilder(HINSTANCE hInstance,
                        int nCmdShow,
                        const std::string& appName,
                        unsigned int winWidth,
                        unsigned int winHeight,
                        bool windowed,
                        HICON icon);

   HWND create(IWindowMessagesProcessor& winMsgProcessor);
};

///////////////////////////////////////////////////////////////////////////////
