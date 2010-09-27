#pragma once

/// @file   TamyEditor\DebugRenderer.h
/// @brief  a debug scene allowing to display all sorts of 
///         additional visual information

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Model;
class Renderer;
class ResourcesManager;
class Camera;
class LineSegments;
class Geometry;
class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

typedef unsigned int DebugHandle;

///////////////////////////////////////////////////////////////////////////////

class DebugRenderer
{
private:
   Renderer&                        m_renderer;
   ResourcesManager&                m_resMgr;
   Camera&                          m_camera;
   Model*                           m_localModel;

   // entities
   std::vector< SpatialEntity* >    m_entities;
   std::vector< DebugHandle >       m_freeHandles;

public:
   DebugRenderer( Renderer& renderer, ResourcesManager& resMgr, Camera& camera );
   ~DebugRenderer();

   /**
    * Returns the underlying scene the renderer operates on.
    */
   inline Model& getModel() { return *m_localModel; }

   /**
    * Adds a new mesh to the debug view
    */
   DebugHandle drawMesh( Geometry* geometry );

   /**
    * Adds a spatial entity to the debug view.
    */
   DebugHandle drawEntity( SpatialEntity* entity );

   /**
    * Removes an object from being drawn.
    */
   void stopDrawing( DebugHandle handle );

private:
   LineSegments* createGrid() const;
};

///////////////////////////////////////////////////////////////////////////////
