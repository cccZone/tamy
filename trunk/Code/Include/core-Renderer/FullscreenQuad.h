/// @file   core-Renderer/FullscreenQuad.h
/// @brief  full screen quad geometry
#pragma once

#include "core-Renderer\Renderable.h"
#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"
#include "core\Observer.h"


///////////////////////////////////////////////////////////////////////////////

class FullscreenQuadImpl;
class ResourcesManager;
class Renderer;
enum RendererOps;

///////////////////////////////////////////////////////////////////////////////

/**
 * Full screen quad geometry.
 */
class FullscreenQuad : public Renderable, 
                       public TRendererObject< FullscreenQuadImpl >,
                       public Observer< Renderer, RendererOps >
{
   DECLARE_RTTI_CLASS

private:
   Renderer*                  m_renderer;
   Attributes                 m_attributes;

   unsigned int               m_width;
   unsigned int               m_height;

public:
   /**
    * Constructor.
    *
    * @param renderer
    */
   FullscreenQuad( Renderer* renderer = NULL );
   ~FullscreenQuad();

   /**
    * Initializes the quad
    * 
    * @param rm         resources manager
    */
   void initialize( ResourcesManager& rm );

   /**
    * Returns the width of the quad.
    */
   inline unsigned int getWidth() const { return m_width; }

   /**
    * Returns the height of the quad.
    */
   inline unsigned int getHeight() const { return m_height; }

   // -------------------------------------------------------------------------
   // Renderer observer implementation
   // -------------------------------------------------------------------------
   void update(Renderer& renderer);
   void update(Renderer& renderer, const RendererOps& operation);

   // -------------------------------------------------------------------------
   // Renderable implementation
   // -------------------------------------------------------------------------
   void render();
   const Attributes& getAttributes() const { return m_attributes; }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Graphics library dependent implementation of a full-screen quad.
 */
class FullscreenQuadImpl : public RendererObjectImpl
{
public:
   virtual ~FullscreenQuadImpl() {}

   /**
    * Renders the full-screen quad geometry.
    */
   virtual void render() {}
};

///////////////////////////////////////////////////////////////////////////////
