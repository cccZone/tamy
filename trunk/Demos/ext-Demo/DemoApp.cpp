#include "ext-Demo\DemoApp.h"
#include "tamy\Tamy.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-AppFlow\UserInputController.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\DynamicNodesStorage.h"
#include "core-Renderer\LightsStorage.h"
#include "core-Renderer\RenderingPipelineBuilder.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\DefaultDemoInputController.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

DemoApp::DemoApp(Tamy& tamy)
: Application("Demo")
, m_tamy(tamy)
, m_renderer(&(tamy.renderer()))
, m_uiController(m_tamy.uiController())
, m_demoController(NULL)
, m_camera(NULL)
{
   timeController().add("regularTrack");
   timeController().get("regularTrack").add(new TTimeDependent<DemoApp>(*this));
   timeController().get("regularTrack").add(new TTimeDependent<UserInputController>(m_uiController));

   m_rendererDefinition = new DemoRendererDefinition(m_tamy.graphicalFactory());

   m_dynamicMeshesScene = new DynMeshesScene(new Octree<RenderableNode>(64, 2000));
   m_lightsScene = new LightsScene();
}

///////////////////////////////////////////////////////////////////////////////

DemoApp::~DemoApp()
{
   m_dynamicMeshesScene = NULL;
   m_lightsScene = NULL;

   delete m_rendererDefinition;
   m_rendererDefinition = NULL;

   delete m_camera;
   m_camera = NULL;

   delete m_demoController;
   m_demoController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

RenderingMechanism* DemoApp::initRenderingPipeline(DemoRendererDefinition& rendererDefinition,
                                                   DynMeshesScene* dynamicScene, 
                                                   LightsScene* lights)
{
   rendererDefinition.addSource("scene", new DynamicNodesStorage(dynamicScene));
   rendererDefinition.aliasMechanism("<<fixedPipeline>>", "backgroundRenderer");
   rendererDefinition.aliasMechanism("<<fixedPipeline>>", "fixedSceneRenderer");
   rendererDefinition.addLightsForMechanism("fixedSceneRenderer", lights);

   m_camera = m_tamy.graphicalFactory().createCamera("camera");
   rendererDefinition.defineCamera("backgroundRenderer", *m_camera);
   rendererDefinition.defineCamera("fixedSceneRenderer", *m_camera);

   RenderingPipelineBuilder builder(rendererDefinition);
   RenderingMechanism* sceneRenderer = builder
      .defineTransform("backgroundRenderer")
         .in("<<background>>")
         .out("<<screen>>")
      .end()
      .defineTransform("fixedSceneRenderer")
         .dep("backgroundRenderer")
         .in("scene")
         .out("<<screen>>")
      .end()
   .end();

   return sceneRenderer;
}

///////////////////////////////////////////////////////////////////////////////

void DemoApp::initInput(UserInputController& uiController)
{
   ASSERT(m_camera != NULL, "Default demo camera not initialized");
   m_demoController = new DefaultDemoInputController(uiController, 
                                                     *m_camera, 
                                                     EndDemoCommand::from_method<DemoApp, &DemoApp::endDemo>(this));
   timeController().get("regularTrack").add(new TTimeDependent<DefaultDemoInputController>(*m_demoController));
}

///////////////////////////////////////////////////////////////////////////////

void DemoApp::setBackground(SkyBoxStorage* skyBox)
{
   m_rendererDefinition->setBackground(skyBox);
}

///////////////////////////////////////////////////////////////////////////////

void DemoApp::initialize()
{
   initializeScene(*m_dynamicMeshesScene, *m_lightsScene);

   RenderingMechanism* mech = initRenderingPipeline(*m_rendererDefinition, 
                                                    m_dynamicMeshesScene, 
                                                    m_lightsScene);

   m_renderer->setMechanism(mech);

   initInput(m_uiController);
}

///////////////////////////////////////////////////////////////////////////////

void DemoApp::deinitialize()
{  
   onDeinitialize();
}

///////////////////////////////////////////////////////////////////////////////

void DemoApp::update(float timeElapsed)
{
   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

void DemoApp::initDefaultCamera(Camera* camera)
{
   ASSERT (m_camera == NULL, "Default demo camera already initialized");
   m_camera = camera;
}

///////////////////////////////////////////////////////////////////////////////

void DemoApp::endDemo()
{
   context().signal(*this, ON_EXIT);
}

///////////////////////////////////////////////////////////////////////////////
