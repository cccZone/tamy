/// @file   core-Renderer\RenderingView.h
/// @brief  view that renders renderable entities
#pragma once

#include <vector>
#include "core-MVC\ModelView.h"
#include "core\RegularOctree.h"
#include "core\AABoundingBox.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class Geometry;
class Light;
class Camera;
class RenderState;
class SceneRenderTreeBuilder;
class MemoryPool;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

// TODO: !!!!!!!!!! DEBUG ENTITIES DISSAPEAR, because the octree used in the rendering view is static and does not recompute their positions.

/**
* This view manages the visibility of renderables.
*/ 
class RenderingView : public ModelView
{
private:
   Renderer&                                                m_renderer;

   MemoryPool*                                              m_treeMemPool;
   RegularOctree< Geometry >*                               m_geometryStorage;
   RegularOctree< Light >*                                  m_lightsStorage;

   AABoundingBox                                            m_sceneBounds;

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
    * Collects renderables from the specified bounding volume
    *
    * @param volume
    * @param outVisibleElems
    */
   void collectRenderables( const BoundingVolume& volume, Array< Geometry* >& outVisibleElems ) const;

   /**
    * Collects visible lights that affect the scene and should be rendered this frame.
    *
    * @param outVisibleLights
    */
   void collectLights( Array< Light* >& outVisibleLights );

   /**
    * Collects lights from the specified bounding volume
    *
    * @param volume
    * @param outVisibleLights
    */
   void collectLights( const BoundingVolume& volume, Array< Light* >& outVisibleLights ) const;

   /**
    * Returns the scene bounds.
    */
   inline const AABoundingBox& getSceneBounds() const { return m_sceneBounds; }

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
