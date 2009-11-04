#include "HierarchicalObjectDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\SkyBoxStorage.h"
#include "ext-Demo\StaticSceneManager.h"
#include "ext-Demo\BasicRenderingPipeline.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\SharedOverlay.h"
#include "ext-Demo\RERCreator.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

HierarchicalObjectDemo::HierarchicalObjectDemo(Tamy& tamy)
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

void HierarchicalObjectDemo::initialize()
{   
   SkyBoxStorage* skyBox = NULL;
   StaticSceneManager* staticSceneStorage = NULL;
   DynMeshesScene* dynamicSceneStorage = NULL;
   LightsScene* lights = NULL;
   loadIWF("Space_Scene.iwf", &skyBox, &staticSceneStorage, &dynamicSceneStorage, &lights);

   // assemble rendering pipeline
   m_renderingPipeline->setBackground(skyBox);
   m_renderingPipeline->setStaticScene(staticSceneStorage);
   m_renderingPipeline->setDynamicScene(dynamicSceneStorage);
   m_renderingPipeline->setLights(lights);
   m_renderingPipeline->setOverlay(new demo::SharedOverlay(getFpsView()));
   m_renderingPipeline->create();

   // initialize input controller
   createDefaultInput(*m_camera);
}

///////////////////////////////////////////////////////////////////////////////

void HierarchicalObjectDemo::deinitialize()
{
   timeController().remove("rendererTrack");

   delete m_renderingPipeline;
   m_renderingPipeline = NULL;

   delete m_camera;
   m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

DEMO(HierarchicalObjectDemo)

///////////////////////////////////////////////////////////////////////////////
