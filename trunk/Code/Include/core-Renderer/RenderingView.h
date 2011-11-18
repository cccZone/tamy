#pragma once

/// @file   core-Renderer\RenderingView.h
/// @brief  view that renders renderable entities

#include <map>
#include <vector>
#include "core-MVC\ModelView.h"
#include "core/RegularOctree.h"


///////////////////////////////////////////////////////////////////////////////

class SpatialRepresentation;
class Renderer;
class Geometry;
struct AABoundingBox;
class Camera;
class RenderState;
class SceneRenderTreeBuilder;
class MemoryPool;

///////////////////////////////////////////////////////////////////////////////

/**
* This view manages the visibility of renderables.
*/ 
class RenderingView : public ModelView
{
private:
   typedef std::map< Geometry*, SpatialRepresentation* >    SpatialsMap;

private:
   Renderer&                                                m_renderer;

   MemoryPool*                                              m_treeMemPool;
   SpatialsMap                                              m_spatials;
   RegularOctree< SpatialRepresentation >*                  m_storage;

public:
   /**
    * Constructor.
    */
   RenderingView( Renderer& renderer, const AABoundingBox& sceneBB );
   ~RenderingView();

   /**
    * Renders the view contents.
    *
    * @param treeBuilder      scene tree builder strategy
    */
   void render( const SceneRenderTreeBuilder& treeBuilder );

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
