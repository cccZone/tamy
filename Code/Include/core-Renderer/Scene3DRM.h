#pragma once

/// @file   core-Renderer\Scene3DRM.h
/// @brief  3D scene rendering mechanism

#include "core-Renderer\RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class Model;
class RenderingView;
class SpatialView;
class CameraContext;
class Camera;

///////////////////////////////////////////////////////////////////////////////

class Scene3DRM : public RenderingMechanism
{
private:
   Model& m_scene;
   RenderingView* m_renderingView;
   SpatialView* m_spatialView;
   CameraContext* m_camera;

public:
   /**
    * Constructor.
    *
    * @param scene   renderable scene
    * @param camera  camera that renders the scene
    */
   Scene3DRM(Model& scene, Camera& camera);
   ~Scene3DRM();

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void render();
};

///////////////////////////////////////////////////////////////////////////////
