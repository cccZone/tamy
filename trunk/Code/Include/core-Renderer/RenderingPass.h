#pragma once

/// @file   core-Renderer\RenderingPass.h
/// @brief  A single rendering pass in the rendering pipeline

#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"
#include "core-Renderer\RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPassImpl;
class AttributeSorter;
class RenderTarget;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A single rendering pass features rendering a set of objects to a specified
 * rendering target. 
 * The objects will internally be managed by a single states manager ( a sorter ).
 *
 * A pass is a regular rendering mechanism, which can be placed in a composite 
 * along with other mechanisms to create a more sophisticated mechanism.
 *
 * This is just an abstract mechanism - its implementations are the ones 
 * that take care of managing objects that will eventually be rendered by it
 */
class RenderingPass : public TRendererObject< RenderingPassImpl >,  
                      public RenderingMechanism
{
   DECLARE_RTTI_CLASS

private:
   Renderer*                  m_renderer;
   AttributeSorter*           m_statesManager;
   RenderTarget*              m_renderTarget;

public:
   /**
    * Constructor.
    *
    * @param renderTarget     render target this pass should render to
    *                         (or NULL if we want to render directly to 
    *                         the back buffer)
    *                         The renderTarget's lifetime will not be managed.
    */
   RenderingPass( RenderTarget* renderTarget = NULL );
   virtual ~RenderingPass();

   /**
    * Returns a render target this pass uses.
    */
   inline const RenderTarget* getRenderTarget() const { return m_renderTarget; }

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void render();

protected:
   /**
    * Returns instance of the states manager this pass uses.
    */
   inline AttributeSorter& statesMgr() { return *m_statesManager; }

   // -------------------------------------------------------------------------
   // Notifications
   // -------------------------------------------------------------------------
   /**
    * Called before the rendering pass started.
    *
    * @return  'true' if the pass should be rendered, false otherwise
    */
   virtual bool onPreRender() { return true; }

   /**
    * Method called once the rendering pass is complete and the rendering target
    * is updated with a fresh image of the rendered scene.
    */
   virtual void onPostRender() {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pass implementation.
 */
class RenderingPassImpl : public RendererObjectImpl
{
public:
   virtual ~RenderingPassImpl() {}

   /**
    * Begins a rendering pass.
    */
   virtual void passBegin() {}

   /**
    * Ends a rendering pass.
    */
   virtual void passEnd() {}
};

///////////////////////////////////////////////////////////////////////////////
