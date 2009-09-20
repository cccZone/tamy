#pragma once

#include "core-Renderer\RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingTargetsPolicy;
class Overlay;

///////////////////////////////////////////////////////////////////////////////

/**
 * This rendering mechanism allows to render screen overlay elements
 * (2d elements with some text etc. - like huds...)
 */
class OverlayRenderingMechanism : public RenderingMechanism
{
private:
   RenderingTargetsPolicy* m_policy;
   Overlay* m_overlay;

public:
   /** 
    * Constructor.
    *
    * @param policy     policy for rendering targets this mechanism should use
    */
   OverlayRenderingMechanism(RenderingTargetsPolicy* policy);

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
