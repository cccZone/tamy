/// @file   core-Renderer/Overlay.h
/// @brief  a 2D element to be displayed in screen-space ( on a HUD i.e. )
#pragma once

#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * An overlay is a 2D element displayed on the screen 
 * (a hud, a message box etc.)
 */
class Overlay
{
public:
   virtual ~Overlay() {}

   /**
    * Renders the overlay on the currently set rendering target.
    */
   virtual void render() = 0;
};

///////////////////////////////////////////////////////////////////////////////
