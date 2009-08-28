#pragma once

#include "ext-Demo\DemoApp.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;

namespace demo
{
   class DemoRendererDefinition;
};

///////////////////////////////////////////////////////////////////////////////

class ComplexSceneDemo : public demo::DemoApp
{
private:
   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;
   Camera* m_camera;

public:
   ComplexSceneDemo(Tamy& tamy);

protected:
   RenderingMechanism* initRenderingPipeline(demo::DemoRendererDefinition& rendererDefinition,
                                             demo::DynMeshesScene* dynamicScene, 
                                             demo::LightsScene* lights);
   void initializeScene(demo::DynMeshesScene& dynamicScene, 
                        demo::LightsScene& lights);
   void onDeinitialize();
};

///////////////////////////////////////////////////////////////////////////////
