#pragma once

#include "ext-Demo\DemoApp.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;

namespace demo
{
   class BasicRenderingPipeline;
}

///////////////////////////////////////////////////////////////////////////////

class TransparencyDemo : public demo::DemoApp
{
private:
   Camera* m_camera;
   demo::BasicRenderingPipeline* m_renderingPipeline;

public:
   TransparencyDemo(Tamy& tamy);

   void initialize();
   void deinitialize();
};

///////////////////////////////////////////////////////////////////////////////
