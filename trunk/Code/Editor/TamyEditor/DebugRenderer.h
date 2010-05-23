#pragma once

/// @file   TamyEditor\DebugRenderer.h
/// @brief  a debug scene allowing to display all sorts of 
///         additional visual information


///////////////////////////////////////////////////////////////////////////////

class Model;
class Renderer;
class Camera;
class LineSegments;
class Geometry;

///////////////////////////////////////////////////////////////////////////////

typedef unsigned int DebugHandle;

///////////////////////////////////////////////////////////////////////////////

class DebugRenderer
{
private:
   Renderer&            m_renderer;
   Camera&              m_camera;
   Model*               m_localModel;

   // grid components
   LineSegments*        m_gridLines;

public:
   DebugRenderer( Renderer& renderer, Camera& camera );
   ~DebugRenderer();

   /**
    * Returns the underlying scene the renderer operates on.
    */
   inline Model& getModel() { return *m_localModel; }

   /**
    * Adds a new mesh to the debug view
    */
   DebugHandle drawMesh( Geometry& geometry );

   /**
    * Removes an object from being drawn.
    */
   void stopDrawing( DebugHandle handle );

private:
   LineSegments* createGrid() const;
};

///////////////////////////////////////////////////////////////////////////////
