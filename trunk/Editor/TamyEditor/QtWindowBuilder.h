#pragma once

/// @file   TamyEditor\TamyWindowBuilder.h
/// @brief  a builder that will create a window required by tamy
///         using Qt GUI system

#include "tamy\TamyWindowBuilder.h"


///////////////////////////////////////////////////////////////////////////////

class TamySceneWidget;

///////////////////////////////////////////////////////////////////////////////

class QtWindowBuilder : public TamyWindowBuilder
{
private:
   TamySceneWidget& m_window;

public:
   /**
    * Constructor.
    *
    * @param window  a widget that should be used as rendering target
    */
   QtWindowBuilder(TamySceneWidget& window);

   // -------------------------------------------------------------------------
   // TamyWindowBuilder implementation
   // -------------------------------------------------------------------------
   HWND create(IWindowMessagesProcessor& winMsgProcessor);
};

///////////////////////////////////////////////////////////////////////////////
