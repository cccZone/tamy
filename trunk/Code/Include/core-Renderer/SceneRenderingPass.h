#pragma once

/// @file   core-Renderer\SceneRenderingPass.h
/// @brief  a rendering pass that can render a visual representation 
///         of multiple scenes

#include "core-Renderer\RenderingPass.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class RenderingView;
class Model;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pass that can render a visual representation of multiple scenes.
 */
class SceneRenderingPass : public RenderingPass
{
private:
   RenderingView*             m_renderingView;

   std::vector< Model* >      m_renderedScenes;

public:
   /**
    * Constructor.
    *
    * @param renderTarget
    */
   SceneRenderingPass( RenderTarget* renderTarget = NULL );
   ~SceneRenderingPass();

   /**
    * Adds a new scene to the rendering pass.
    */
   void addScene( Model& model );

   /**
    * Removes a scene from the rendering pass.
    */
   void removeScene( Model& model );
};

///////////////////////////////////////////////////////////////////////////////
