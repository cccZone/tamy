#pragma once

#include "ext-Demo\DemoApp.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Skeleton;
class Camera;
class PostProcessPipeline;

///////////////////////////////////////////////////////////////////////////////

class EffectsDemo : public demo::DemoApp
{
private:
   Camera* m_camera;
   PostProcessPipeline* m_renderingPipeline;
   Skeleton* m_animationController;

public:
   EffectsDemo(Tamy& tamy);

   void initialize();
   void deinitialize();
};

///////////////////////////////////////////////////////////////////////////////
