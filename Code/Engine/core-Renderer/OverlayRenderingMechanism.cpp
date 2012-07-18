#include "core-Renderer\OverlayRenderingMechanism.h"
#include "core-Renderer\Overlay.h"
#include "core\Assert.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

OverlayRenderingMechanism::OverlayRenderingMechanism()
   : m_overlay(NULL)
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
      ASSERT_MSG( false, "NULL pointer instead an Overlay instance" );
      return;
   }

   delete m_overlay;
   m_overlay = overlay;
}

///////////////////////////////////////////////////////////////////////////////

void OverlayRenderingMechanism::initialize( Renderer& renderer )
{

}

///////////////////////////////////////////////////////////////////////////////

void OverlayRenderingMechanism::deinitialize( Renderer& renderer )
{

}

///////////////////////////////////////////////////////////////////////////////

void OverlayRenderingMechanism::render( Renderer& renderer )
{
   if (m_overlay != NULL) 
   {
      m_overlay->render();
   }
}

///////////////////////////////////////////////////////////////////////////////
