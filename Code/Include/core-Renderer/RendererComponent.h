#pragma once

/// @file   core-Renderer\RendererComponent.h
/// @brief  A component with an active renderer instance.

#include "core\Component.h"
#include "core-Renderer\Renderer.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

class RendererComponent : public Component< ResourcesManager >
{
private:
   Renderer&   m_renderer;

public:
   /**
    * Constructor.
    *
    * @param renderer
    */
   RendererComponent( Renderer& renderer ) : m_renderer( renderer ) {}

   /**
    * Returns a camera instance.
    */
   inline Renderer& getRenderer()  {return m_renderer; }


   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize( ResourcesManager& mgr ) {}
};

///////////////////////////////////////////////////////////////////////////////
