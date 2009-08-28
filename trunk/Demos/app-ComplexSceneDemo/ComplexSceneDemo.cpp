#include "ComplexSceneDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\RenderingPipelineBuilder.h"
#include "core-Renderer\DynamicNodesStorage.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "core-Renderer\LightsSortingStorage.h"
#include "core-Renderer\LightsSorter.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\DemoIWFScene.h"
#include "ext-Demo\RERCreator.h"
#include "RenderEffectController.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

ComplexSceneDemo::ComplexSceneDemo(Tamy& tamy)
: DemoApp(tamy)
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingMechanism* 
ComplexSceneDemo::initRenderingPipeline(DemoRendererDefinition& rendererDefinition,
                                        DynMeshesScene* dynamicScene, 
                                        LightsScene* lights)
{
   rendererDefinition.addSource("scene", new DynamicNodesStorage(dynamicScene));
   rendererDefinition.aliasMechanism("<<fixedPipeline>>", "backgroundRenderer");
   rendererDefinition.aliasMechanism("<<fixedPipeline>>", "fixedSceneRenderer");
   rendererDefinition.aliasMechanism("..\\Data\\renderer.fx", "programmableSceneRenderer");
   rendererDefinition.setProgrammableMechanismLoader("..\\Data\\renderer.fx", new demo::TRERCreator<RenderEffectController>());

   // using a sorted lights storage we're providing for dynamic lights in our scene
   LightsSortingStorage<LightsSorter>* dynamicLightsStorage = new LightsSortingStorage<LightsSorter> (lights);
   rendererDefinition.addLightsForMechanism("fixedSceneRenderer", dynamicLightsStorage);
   rendererDefinition.addLightsForMechanism("programmableSceneRenderer", dynamicLightsStorage);

   Camera* camera = m_tamy.graphicalFactory().createCamera("camera");
   initDefaultCamera(camera);
   rendererDefinition.defineCamera("backgroundRenderer", *camera);
   rendererDefinition.defineCamera("fixedSceneRenderer", *camera);
   rendererDefinition.defineCamera("programmableSceneRenderer", *camera);

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

void ComplexSceneDemo::initializeScene(DynMeshesScene& dynamicScene, 
                                       LightsScene& lights)
{   
   DemoIWFScene sceneAdapter(m_tamy.graphicalFactory(), 
                             m_tamy.meshLoaders(),
                             demo::LightsSceneSetter::from_method<LightsScene, &LightsScene::insert> (&lights),
                             demo::SkyBoxSceneSetter::from_method<demo::DemoApp, &demo::DemoApp::setBackground> (this),
                             demo::DynamicObjectsSceneSetter::from_method<demo::DynMeshesScene, &demo::DynMeshesScene::addNode> (&dynamicScene),
                             m_entitiesStorage,
                             m_materialsStorage);
   IWFLoader loader(sceneAdapter);
   loader.load("..\\Data\\Colony5.iwf");
}

///////////////////////////////////////////////////////////////////////////////

void ComplexSceneDemo::onDeinitialize()
{
}

///////////////////////////////////////////////////////////////////////////////

DEMO(ComplexSceneDemo)

///////////////////////////////////////////////////////////////////////////////
