#include "EffectsDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Skeleton.h"
#include "core-Renderer\RenderingPipelineBuilder.h"
#include "core-Renderer\DynamicNodesStorage.h"
#include "core-Renderer\PostProcessEffectRenderable.h"
#include "core-Renderer\PostProcessSceneStorage.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\DemoIWFScene.h"
#include "ext-Demo\RERCreator.h"
#include "PostProcessRenderer.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

EffectsDemo::EffectsDemo(Tamy& tamy)
: DemoApp(tamy)
{
   timeController().add("animationTrack");
}

///////////////////////////////////////////////////////////////////////////////

RenderingMechanism* EffectsDemo::initRenderingPipeline(DemoRendererDefinition& rendererDefinition,
                                                       DynMeshesScene* dynamicScene, 
                                                       LightsScene* lights)
{
   rendererDefinition.createTarget("target", RT_COLOR);
   Texture& targetTex = rendererDefinition.getTargetAsTexture("target");

   GraphicalEntitiesFactory& factory = m_tamy.graphicalFactory();
   PostProcessEffectRenderable* ppRenderable = factory.createPostProcessEffectRenderable();
   ppRenderable->addInputTexture(targetTex, factory);

   rendererDefinition.addSource("scene", new DynamicNodesStorage(dynamicScene));
   rendererDefinition.addSource("renderedScene", new PostProcessSceneStorage(ppRenderable));
   rendererDefinition.aliasMechanism("<<fixedPipeline>>", "backgroundRenderer");
   rendererDefinition.aliasMechanism("<<fixedPipeline>>", "fixedSceneRenderer");
   rendererDefinition.aliasMechanism("..\\Data\\oldTV.fx", "postProcessRenderer");
   rendererDefinition.setProgrammableMechanismLoader("..\\Data\\oldTV.fx", new demo::TRERCreator<PostProcessRenderer>());
   rendererDefinition.addLightsForMechanism("fixedSceneRenderer", lights);

   Camera* camera = m_tamy.graphicalFactory().createCamera("camera");
   initDefaultCamera(camera);
   rendererDefinition.defineCamera("backgroundRenderer", *camera);
   rendererDefinition.defineCamera("fixedSceneRenderer", *camera);
   rendererDefinition.defineCamera("postProcessRenderer", *camera);

   RenderingPipelineBuilder builder(rendererDefinition);
   RenderingMechanism* sceneRenderer = builder
      .defineTransform("backgroundRenderer")
         .in("<<background>>")
         .out("target")
      .end()
      .defineTransform("fixedSceneRenderer")
         .dep("backgroundRenderer")
         .in("scene")
         .out("target")
      .end()
      .defineTransform("postProcessRenderer")
         .dep("fixedSceneRenderer")
         .in("renderedScene")
         .out("<<screen>>")
      .end()
   .end();


   return sceneRenderer;
}

///////////////////////////////////////////////////////////////////////////////

void EffectsDemo::initializeScene(DynMeshesScene& dynamicScene, 
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
   loader.load("..\\Data\\AnimLandscape.iwf");

   AbstractGraphicalEntity& ent = m_entitiesStorage.get("animlandscape.x");
   m_animationController = ent.instantiateSkeleton(dynamicScene.root());
   m_animationController->activateAnimation("Cutscene_01", true);
   timeController().get("animationTrack").add(new TTimeDependent<Skeleton>(*m_animationController));

   Light* light = m_tamy.graphicalFactory().createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 0));
   light->setLookVec(D3DXVECTOR3(0, 0, -1));
   lights.insert(light);
}

///////////////////////////////////////////////////////////////////////////////

void EffectsDemo::onDeinitialize()
{   
   timeController().remove("animationTrack");

   delete m_animationController;
   m_animationController = NULL;
}


///////////////////////////////////////////////////////////////////////////////

DEMO(EffectsDemo)

///////////////////////////////////////////////////////////////////////////////
