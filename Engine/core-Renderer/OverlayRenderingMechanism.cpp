#include "core-Renderer\OverlayRenderingMechanism.h"
#include "core-Renderer\Overlay.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

OverlayRenderingMechanism::OverlayRenderingMechanism(RenderingTargetsPolicy* policy)
: m_policy(policy)
, m_overlay(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

OverlayRenderingMechanism::~OverlayRenderingMechanism()
{
   delete m_policy;
   m_policy = NULL;
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
      m_policy->setTargets(0);
      m_overlay->render();
   }
}

///////////////////////////////////////////////////////////////////////////////
