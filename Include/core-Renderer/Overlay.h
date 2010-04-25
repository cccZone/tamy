#pragma once


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
