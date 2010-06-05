#pragma once

/// @file   TamyEditor\InputCommand.h
/// @brief  a command executed by pointing mouse at something and clicking

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

enum InputState
{
   IS_PRESSED,
   IS_HELD,
   IS_RELEASED,
};

///////////////////////////////////////////////////////////////////////////////

class InputCommand
{
public:
   virtual ~InputCommand() {}

   /**
    * Executes the command.
    *
    * @param state      state the triggering button is in
    * @param mousePos   position of the mouse pointer at the time the command
    *                   is executed (in viewport coordinates).
    */
   virtual void execute( InputState state, const D3DXVECTOR2& mousePos ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
