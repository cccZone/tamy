#pragma once

#include "ext-Demo\DemoApp.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Skeleton;
class RenderingMechanism;

namespace demo
{
   class DemoRendererDefinition;
};

///////////////////////////////////////////////////////////////////////////////

class EffectsDemo : public demo::DemoApp
{
private:
   Skeleton* m_animationController;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;

public:
   EffectsDemo(Tamy& tamy);

protected:
   RenderingMechanism* initRenderingPipeline(demo::DemoRendererDefinition& rendererDefinition,
                                             demo::DynMeshesScene* dynamicScene, 
                                             demo::LightsScene* lights);
   void initializeScene(demo::DynMeshesScene& dynamicScene, demo::LightsScene& lights);
   void onDeinitialize();
};

///////////////////////////////////////////////////////////////////////////////
