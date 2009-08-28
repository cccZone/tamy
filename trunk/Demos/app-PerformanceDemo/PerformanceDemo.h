#pragma once

#include "ext-Demo\DemoApp.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

namespace demo
{
   class DemoRendererDefinition;
};

///////////////////////////////////////////////////////////////////////////////

class PerformanceDemo : public demo::DemoApp
{
private:
   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;

public:
   PerformanceDemo(Tamy& tamy);

protected:
   void initializeScene(demo::DynMeshesScene& dynamicScene, 
                        demo::LightsScene& lights);
   void onDeinitialize();
};

///////////////////////////////////////////////////////////////////////////////
