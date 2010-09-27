#pragma once

/// @file   core-Renderer\VisibilityPass.h
/// @brief  checks the visibility of objects on the added scenes.

#include "core-Renderer\RenderingMechanism.h"
#include "core-MVC\ModelComponent.h"
#include <map>


///////////////////////////////////////////////////////////////////////////////

class Model;
class SpatialView;
class CameraContext;
class Camera;

///////////////////////////////////////////////////////////////////////////////

/**
 * Checks the visibility of objects on the added scenes. Best set it as the very
 * first rendering pass.
 */
class VisibilityPass : public RenderingMechanism
{
private:
   typedef std::map< Model*, ModelComponent< Camera >* > ScenesMap;

private:
   SpatialView*               m_spatialView;
   Camera&                    m_camera;
   CameraContext*             m_cameraContext;

   ScenesMap                  m_renderedScenes;

public:
   /**
    * Constructor.
    *
    * @param camera  camera that renders the scene
    */
   VisibilityPass( Camera& camera );
   ~VisibilityPass();

   /**
    * Adds a new scene to the visibility pass.
    */
   void addScene( Model& model );

   /**
    * Removes a scene from the visibility pass.
    */
   void removeScene( Model& model );

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer ) {}
   void render();
};

///////////////////////////////////////////////////////////////////////////////
