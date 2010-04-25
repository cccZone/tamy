#pragma once

#include "ext-Demo\DemoApp.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Skeleton;
class Camera;

namespace demo
{
   class BasicRenderingPipeline;
}

///////////////////////////////////////////////////////////////////////////////

class TreesGeneratorDemo : public demo::DemoApp
{
private:
   Camera* m_camera;
   demo::BasicRenderingPipeline* m_renderingPipeline;

   AbstractGraphicalEntity* m_treeEntity;
   Skeleton* m_animationController;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;

public:
   TreesGeneratorDemo(Tamy& tamy);

   void initialize();
   void deinitialize();
};

///////////////////////////////////////////////////////////////////////////////
