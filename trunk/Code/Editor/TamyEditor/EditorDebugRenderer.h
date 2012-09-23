/// @file   TamyEditor/EditorDebugRenderer.h
/// @brief  debug renderer for the editor-related entities
#pragma once

#include "core/Array.h"
#include "core-Renderer/RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class Model;
class RenderingView;
class StatefulRenderTreeBuilder;
class Geometry;
class MemoryPool;
class Entity;
class ModelView;
class DebugEntitiesManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * Debug renderer for the editor-related entities.
 */
class EditorDebugRenderer : public RenderingMechanism
{
private:
   typedef Array< Geometry* >                      GeometryArray;

   DebugEntitiesManager*                           m_host;

   StatefulRenderTreeBuilder*                      m_renderTreeBuilder;
   MemoryPool*                                     m_treeMemPool;
   Model*                                          m_debugScene;
   RenderingView*                                  m_renderingView;
   GeometryArray*                                  m_visibleElems;

   Array< ModelView* >                             m_externalSceneViews;
   Array< Entity* >                                m_entitiesToAdd;

public:
   /**
    * Constructor.
    *
    * @param host
    */
   EditorDebugRenderer();
   ~EditorDebugRenderer();

   /**
    * Sets the host DebugEntitiesManager instance the state of this renderer is controlled by.
    *
    * @param host
    */
   void setHost( DebugEntitiesManager& host );

   /**
    * Adds a new entity for debug rendering.
    *
    * @param entity
    */
   void add( Entity& entity );

   /**
    * Adds a new entity for debug rendering.
    *
    * @param entity
    */
   void remove( Entity& entity );

   /**
    * Attaches a model view to the debug scene.
    *
    * @param view
    */
   void attachSceneView( ModelView& view );

   /**
    * Attaches a model view to the debug scene.
    *
    * @param view
    */
   void detachSceneView( ModelView& view );

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void deinitialize( Renderer& renderer );
   void render( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
