#pragma once

/// @file   TamyEditor\CameraCommand.h
/// @brief  a command executed by pointing mouse at something and clicking

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class CameraCommand
{
public:
   virtual ~CameraCommand() {}

   /**
    * Executes the command.
    *
    * @param mousePos   position of the mouse pointer at the time the command
    *                   is executed (in viewport coordinates).
    */
   virtual void execute(const D3DXVECTOR2& mousePos) = 0;
};

///////////////////////////////////////////////////////////////////////////////
