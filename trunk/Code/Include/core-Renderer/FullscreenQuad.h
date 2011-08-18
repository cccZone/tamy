/// @file   core-Renderer/FullscreenQuad.h
/// @brief  full screen quad geometry
#pragma once

#include "core-Renderer/RenderCommand.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Graphics library dependent implementation of a full-screen quad.
 */
class RCFullscreenQuad : public RenderCommand
{
private:
   unsigned int m_width;
   unsigned int m_height;

public:
   RCFullscreenQuad( unsigned int width, unsigned int height );

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
