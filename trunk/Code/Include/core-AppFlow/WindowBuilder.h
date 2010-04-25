#pragma once

/// @file   core-AppFlow\WindowBuilder.h
/// @brief  generic interface for a windows creator

#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class IWindowMessagesProcessor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Window builder should create a window and plug a message processor
 * into it. The message processor will receive the messages about 
 * window events (resizing, moving, mouse & keyboard clicks etc.)
 */
class WindowBuilder
{
public:
   virtual ~WindowBuilder() {}

   /**
    * The method creates a window and plugs a message processor
    * into it.
    *
    * @param   winMsgProcessor
    * @return  window handle
    */
   virtual HWND create(IWindowMessagesProcessor& winMsgProcessor); 
};

///////////////////////////////////////////////////////////////////////////////
