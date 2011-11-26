#pragma once

/// @file   core-Renderer\RenderingView.h
/// @brief  view that renders renderable entities

#include <map>
#include <vector>
#include "core-MVC\ModelView.h"
#include "core/RegularOctree.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class Geometry;
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
   RegularOctree< Geometry >*                               m_storage;

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
