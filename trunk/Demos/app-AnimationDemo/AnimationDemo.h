#pragma once

#include "ext-Demo\DemoApp.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Skeleton;

namespace demo
{
   class DemoRendererDefinition;
};

///////////////////////////////////////////////////////////////////////////////

class AnimationDemo : public demo::DemoApp
{
private:
   Skeleton* m_animationController;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;

public:
   AnimationDemo(Tamy& tamy);

protected:
   void initializeScene(demo::DynMeshesScene& dynamicScene, demo::LightsScene& lights);
   void onDeinitialize();
};

///////////////////////////////////////////////////////////////////////////////
