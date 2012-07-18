/// @file   core-Renderer\RenderingView.h
/// @brief  view that renders renderable entities
#pragma once

#include <vector>
#include "core-MVC\ModelView.h"
#include "core/RegularOctree.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class Geometry;
class Light;
struct AABoundingBox;
class Camera;
class RenderState;
class SceneRenderTreeBuilder;
class MemoryPool;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

/**
* This view manages the visibility of renderables.
*/ 
class RenderingView : public ModelView
{
private:
   Renderer&                                                m_renderer;

   MemoryPool*                                              m_treeMemPool;
   RegularOctree< Geometry >*                               m_geometryStorage;
   RegularOctree< Light >*                                m_lightsStorage;

public:
   /**
    * Constructor.
    */
   RenderingView( Renderer& renderer, const AABoundingBox& sceneBB );
   ~RenderingView();

   /**
    * Collects renderables that should be rendered this frame.
    *
    * @param outVisibleElems
    */
   void collectRenderables( Array< Geometry* >& outVisibleElems );

   /**
    * Collects visible lights that affect the scene and should be rendered this frame.
    *
    * @param outVisibleLights
    */
   void collectLights( Array< Light* >& outVisibleLights );

   // ----------------------------------------------------------------------
   // ModelView implementation
   // ----------------------------------------------------------------------
   void onEntityAdded( Entity& entity );
   void onEntityRemoved( Entity& entity );
   void onEntityChanged( Entity& entity );

protected:
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////
