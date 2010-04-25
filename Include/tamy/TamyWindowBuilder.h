#pragma once

/// @file   tamy\TamyWindowBuilder.h
/// @brief  tool that will prepare a window where tamy will be rendering stuff

#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class IWindowMessagesProcessor;

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface allows tamy to query for a window it requires in order to
 * correctly create its subsystems.
 */
class TamyWindowBuilder
{
public:
   virtual ~TamyWindowBuilder() {}

   /**
    * This method should create a window and hook the specified
    * message processor to it.
    * 
    * @param winMsgProcessor  messages processor the created window should use
    *                         to inform tamy about window resizes and such...
    * @return                 a handle to newly created window
    */
   virtual HWND create(IWindowMessagesProcessor& winMsgProcessor) = 0;
};

///////////////////////////////////////////////////////////////////////////////
