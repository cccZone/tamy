#pragma once

#include "ext-Demo\DemoApp.h"


///////////////////////////////////////////////////////////////////////////////

class Skeleton;
class Camera;

namespace demo
{
   class BasicRenderingPipeline;
}

///////////////////////////////////////////////////////////////////////////////

class AnimationDemo : public demo::DemoApp
{
private:
   Camera* m_camera;
   demo::BasicRenderingPipeline* m_renderingPipeline;
   Skeleton* m_animationController;

public:
   AnimationDemo(Tamy& tamy);

   void initialize();
   void deinitialize();
};

///////////////////////////////////////////////////////////////////////////////
