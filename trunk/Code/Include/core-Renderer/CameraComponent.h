#pragma once

/// @file   core-Renderer\CameraComponent.h
/// @brief  component that introduces a camera into the model

#include "core\Component.h"


///////////////////////////////////////////////////////////////////////////////

class Model;
class Camera;

///////////////////////////////////////////////////////////////////////////////

/**
 * This component allows to attach a single camera to many models at
 * the same time, showing the scenes they contain from the same perspective.
 * This camera will be used by the entities added to the model during their 
 * rendering.
 */
class CameraComponent : public Component<Model>
{
private:
   Camera& m_camera;

public:
   /**
    * Constructor.
    *
    * @param camera
    */
   CameraComponent(Camera& camera) : m_camera(camera) {}

   /**
    * Returns a camera instance.
    */
   inline Camera& getCamera() {return m_camera;}

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize(Model& mgr) {}
};

///////////////////////////////////////////////////////////////////////////////
