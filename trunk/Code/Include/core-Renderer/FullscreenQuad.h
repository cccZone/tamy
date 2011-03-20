/// @file   core-Renderer/FullscreenQuad.h
/// @brief  full screen quad geometry
#pragma once

#include "core-Renderer\Renderable.h"
#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"


///////////////////////////////////////////////////////////////////////////////

class FullscreenQuadImpl;
class ResourcesManager;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Full screen quad geometry.
 */
class FullscreenQuad : public TRendererObject< FullscreenQuadImpl >
{
   DECLARE_RTTI_CLASS

public:
   /**
    * Constructor.
    *
    * @param renderer
    */
   FullscreenQuad();
   ~FullscreenQuad();

   /**
    * Initializes the quad
    * 
    * @param rm         resources manager
    */
   void initialize( ResourcesManager& rm );

   /**
    * Renders the quad on screen.
    *
    * @param width      quad width
    * @param height     quad height
    */
   void render( unsigned int width, unsigned int height );
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
   virtual void render( unsigned int width, unsigned int height ) {}
};

///////////////////////////////////////////////////////////////////////////////
