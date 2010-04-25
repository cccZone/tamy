#pragma once

#include "core-Renderer\RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class Overlay;

///////////////////////////////////////////////////////////////////////////////

/**
 * This rendering mechanism allows to render screen overlay elements
 * (2d elements with some text etc. - like huds...)
 */
class OverlayRenderingMechanism : public RenderingMechanism
{
private:
   Overlay* m_overlay;

public:
   /** 
    * Constructor.
    */
   OverlayRenderingMechanism();
   ~OverlayRenderingMechanism();

   /**
    * This method sets a new overlay that will be from now on
    * rendered by this mechanism.
    *
    * @param overlay    overlay this mechanism should render
    */
   void setOverlay(Overlay* overlay);

   void render();
};

///////////////////////////////////////////////////////////////////////////////
