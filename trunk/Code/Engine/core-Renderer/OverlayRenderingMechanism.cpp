#include "core-Renderer\OverlayRenderingMechanism.h"
#include "core-Renderer\Overlay.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

OverlayRenderingMechanism::OverlayRenderingMechanism()
:  m_overlay(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

OverlayRenderingMechanism::~OverlayRenderingMechanism()
{
}

///////////////////////////////////////////////////////////////////////////////

void OverlayRenderingMechanism::setOverlay(Overlay* overlay)
{
   if (overlay == NULL)
   {
      throw std::runtime_error("NULL pointer instead an Overlay instance");
   }

   delete m_overlay;
   m_overlay = overlay;
}

///////////////////////////////////////////////////////////////////////////////

void OverlayRenderingMechanism::render()
{
   if (m_overlay != NULL) 
   {
      m_overlay->render();
   }
}

///////////////////////////////////////////////////////////////////////////////
