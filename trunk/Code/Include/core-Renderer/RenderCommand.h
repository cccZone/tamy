/// @file   core-Renderer/RenderCommand.h
/// @brief  a render command interface
#pragma once

#include "core\RoundBuffer.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A renderer command issues implementation specific rendering methods.
 */

class RenderCommand : public RoundBufferObject
{
public:
   virtual ~RenderCommand() {}

   /**
    * Executes a rendering command on the specified renderer.
    */
   virtual void execute( Renderer& renderer ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
