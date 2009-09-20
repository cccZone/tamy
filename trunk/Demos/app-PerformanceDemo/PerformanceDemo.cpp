#include "PerformanceDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\SortingRenderablesStorage.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-Demo\BasicRenderingPipeline.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\SharedOverlay.h"
#include "ext-Demo\RERCreator.h"


using namespace demo;


///////////////////////////////////////////////////////////////////////////////

PerformanceDemo::PerformanceDemo(Tamy& tamy)
: DemoApp(tamy)
, m_camera(NULL)
, m_renderingPipeline(NULL)
{
   m_camera = m_tamy.graphicalFactory().createCamera("camera");
   m_renderingPipeline = new BasicRenderingPipeline(m_tamy.graphicalFactory(), m_tamy.renderer(), *m_camera);

   timeController().add("rendererTrack");
   timeController().get("rendererTrack").add(new TTimeDependent<BasicRenderingPipeline> (*m_renderingPipeline));
}

///////////////////////////////////////////////////////////////////////////////

void PerformanceDemo::initialize()
{   
   GraphicalEntityLoader loader(m_tamy.graphicalFactory(), m_materialsStorage);

   // populate the scene
   demo::DynMeshesScene* dynamicScene = new demo::DynMeshesScene(new LinearStorage<RenderableNode>());

   AbstractGraphicalEntity* ent = loader.load("meadowNormalTile.x", m_tamy.meshLoaders());
   m_entitiesStorage.add(ent);
   for (int y = -20; y < 20; ++y)
   {
      for (int x = -20; x < 20; ++x)
      {
         GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tile");
         entInstance->attachEntity(*ent);
         D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), x * 10.f, 0, y * 10.f);

         dynamicScene->addNode(entInstance);
      }
   }

   Light* light = m_tamy.graphicalFactory().createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));
   light->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   D3DXMatrixRotationYawPitchRoll(&(light->accessLocalMtx()), D3DXToRadian(-45), D3DXToRadian(45), 0);
   demo::LightsScene* lights = new demo::LightsScene();
   lights->insert(light);

   // assemble rendering pipeline
   m_renderingPipeline->setDynamicScene(dynamicScene);
   m_renderingPipeline->setLights(lights);
   m_renderingPipeline->setOverlay(new demo::SharedOverlay(getFpsView()));
   m_renderingPipeline->create();

   // initialize input controller
   createDefaultInput(*m_camera);
}

///////////////////////////////////////////////////////////////////////////////

void PerformanceDemo::deinitialize()
{   
   timeController().remove("rendererTrack");

   delete m_renderingPipeline;
   m_renderingPipeline = NULL;

   delete m_camera;
   m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

DEMO(PerformanceDemo)

///////////////////////////////////////////////////////////////////////////////
