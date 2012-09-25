/// @file   core-Renderer/FullscreenQuad.h
/// @brief  full screen quad geometry
#pragma once

#include "core-Renderer/RenderCommand.h"
#include "core/types.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Graphics library dependent implementation of a full-screen quad.
 */
class RCFullscreenQuad : public RenderCommand
{
private:
   uint           m_width;
   uint           m_height;

public:
   RCFullscreenQuad( uint width, uint height );

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Graphics library dependent implementation of a screen-space quad.
 */
class RCScreenQuad : public RenderCommand
{
private:
   uint           m_offsetX;
   uint           m_offsetY;
   uint           m_width;
   uint           m_height;

public:
   RCScreenQuad( uint offsetX, uint offsetY, uint width, uint height );

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
