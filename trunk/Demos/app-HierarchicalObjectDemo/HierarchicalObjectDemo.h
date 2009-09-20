#pragma once

#include "ext-Demo\DemoApp.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;

namespace demo
{
   class BasicRenderingPipeline;
}

///////////////////////////////////////////////////////////////////////////////

class HierarchicalObjectDemo : public demo::DemoApp
{
private:
   Camera* m_camera;
   demo::BasicRenderingPipeline* m_renderingPipeline;

public:
   HierarchicalObjectDemo(Tamy& tamy);

   void initialize();
   void deinitialize();
};

///////////////////////////////////////////////////////////////////////////////
