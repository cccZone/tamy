#include "PerformanceDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\DynamicNodesStorage.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

PerformanceDemo::PerformanceDemo(Tamy& tamy)
: DemoApp(tamy)
{
}

///////////////////////////////////////////////////////////////////////////////

void PerformanceDemo::initializeScene(DynMeshesScene& dynamicScene, 
                                       LightsScene& lights)
{   
   GraphicalEntityLoader loader(m_tamy.graphicalFactory(), m_materialsStorage);

   AbstractGraphicalEntity* ent = loader.load("meadowNormalTile.x", m_tamy.meshLoaders());
   m_entitiesStorage.add(ent);
   for (int y = -20; y < 20; ++y)
   {
      for (int x = -20; x < 20; ++x)
      {
         GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tile");
         entInstance->attachEntity(*ent);
         D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), x * 10.f, 0, y * 10.f);

         dynamicScene.addNode(entInstance);
      }
   }

   Light* light = m_tamy.graphicalFactory().createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));
   light->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   D3DXMatrixRotationYawPitchRoll(&(light->accessLocalMtx()), D3DXToRadian(-45), D3DXToRadian(45), 0);
   lights.insert(light);
}

///////////////////////////////////////////////////////////////////////////////

void PerformanceDemo::onDeinitialize()
{   
}


///////////////////////////////////////////////////////////////////////////////

DEMO(PerformanceDemo)

///////////////////////////////////////////////////////////////////////////////
