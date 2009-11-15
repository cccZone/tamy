#pragma once

/// @file   ext-RendererView\Camera.h
/// @brief  graphical representation of a camera

#include "ext-RendererView\GraphicalRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

struct CameraEntity;
class Camera;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a graphical representation of a camera.
 */
class Camera : public GraphicalRepresentation
{
private:
   CameraEntity& m_entity;
   ::Camera* m_camera;

public:
   /**
    * Constructor.
    *
    * @param entity     camera entity which this class instance 
    *                   represents
    */
   Camera(CameraEntity& entity);
   ~Camera();

   // -------------------------------------------------------------------------
   // GraphicalRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(RendererView& rendererView);

   void deinitialize(RendererView& rendererView);

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
