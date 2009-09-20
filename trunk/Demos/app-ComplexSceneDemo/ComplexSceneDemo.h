#pragma once

#include "ext-Demo\DemoApp.h"
#include "core\SpatialStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;

namespace demo
{
   class BasicRenderingPipeline;
}

///////////////////////////////////////////////////////////////////////////////

class ComplexSceneDemo : public demo::DemoApp
{
private:
   Camera* m_camera;
   demo::BasicRenderingPipeline* m_renderingPipeline;

public:
   ComplexSceneDemo(Tamy& tamy);

   void initialize();
   void deinitialize();
};

///////////////////////////////////////////////////////////////////////////////
