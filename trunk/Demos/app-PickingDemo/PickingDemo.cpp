#include "PickingDemo.h"
#include "tamy\Tamy.h"
#include "core\Assert.h"
#include "core\NodeActionsExecutor.h"
#include "core-AppFlow\UserInputController.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\RenderingPipelineBuilder.h"
#include "core-Renderer\DynamicNodesStorage.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\ParticleSystem.h"
#include "core-Renderer\ParticleFader.h"
#include "core-Renderer\PlanarParticleInitializer.h"
#include "core-Renderer\PointParticleInitializer.h"
#include "core-Renderer\CircularParticleInitializer.h"
#include "core-Renderer\ProjCalc2D.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"
#include "ext-MotionControllers\TimeFunction.h"
#include "ext-MotionControllers\WaypointCameraController.h"
#include "JumpToNodeAction.h"
#include "InputController.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

class LinearTimeFunc : public TimeFunction
{
public:
   float operator()(const float& advancement) const
   {
      ASSERT(advancement <= 1, "The value for advancement should be <= 1");
      ASSERT(advancement >= 0, "The value for advancement should be >= 0");

      return sin(advancement * D3DX_PI / 2.f);
   }
};

///////////////////////////////////////////////////////////////////////////////

PickingDemo::PickingDemo(Tamy& tamy)
: DemoApp(tamy)
, m_actionsExecutor(new NodeActionsExecutor())
, m_inputController(NULL)
{
   timeController().add("animationTrack");
   TimeControllerTrack& animationTrack = timeController().get("animationTrack");

   m_hudMeshesScene = new DynMeshesScene(new Octree<RenderableNode>(64, 2000));
   m_hudLightsScene = new LightsScene();

   m_sceneCamera = m_tamy.graphicalFactory().createCamera("sceneCamera");
   m_cameraController = new WaypointCameraController(*m_sceneCamera, D3DXVECTOR3(0, 10, -30), new LinearTimeFunc());
   animationTrack.add(new TTimeDependent<WaypointCameraController> (*m_cameraController));

   m_hudCamera = m_tamy.graphicalFactory().createCamera("hudCamera");
   m_hudCamera->setProjectionCalculator(new ProjCalc2D());
}

///////////////////////////////////////////////////////////////////////////////

RenderingMechanism* PickingDemo::initRenderingPipeline(DemoRendererDefinition& rendererDefinition,
                                                       DynMeshesScene* dynamicScene, 
                                                       LightsScene* lights)
{
   m_mainScene = dynamicScene;

   rendererDefinition.addSource("scene", new DynamicNodesStorage(dynamicScene));
   rendererDefinition.addSource("hud", new DynamicNodesStorage(m_hudMeshesScene));
   rendererDefinition.aliasMechanism("<<fixedPipeline>>", "hudRenderer");
   rendererDefinition.aliasMechanism("<<fixedPipeline>>", "sceneRenderer");
   rendererDefinition.addLightsForMechanism("hudRenderer", m_hudLightsScene);
   rendererDefinition.addLightsForMechanism("sceneRenderer", lights);

   rendererDefinition.defineCamera("hudRenderer", *m_hudCamera);
   rendererDefinition.defineCamera("sceneRenderer", *m_sceneCamera);

   RenderingPipelineBuilder builder(rendererDefinition);
   RenderingMechanism* sceneRenderer = builder
      .defineTransform("sceneRenderer")
         .in("scene")
         .out("<<screen>>")
      .end()
      .defineTransform("hudRenderer")
         .dep("sceneRenderer")
         .in("hud")
         .out("<<screen>>")
      .end()
   .end();

   return sceneRenderer;
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::initializeScene(DynMeshesScene& dynamicScene, 
                                  LightsScene& lights)
{   
   TimeControllerTrack& animationTrack = timeController().get("animationTrack");

   GraphicalEntitiesFactory& factory = m_tamy.graphicalFactory();
   GraphicalEntityLoader loader(factory, m_materialsStorage);

   AbstractGraphicalEntity* ent = loader.load("meadowNormalTile.x", m_tamy.meshLoaders());
   m_entitiesStorage.add(ent);

   Light* light = factory.createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));
   light->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   D3DXMatrixRotationYawPitchRoll(&(light->accessLocalMtx()), D3DXToRadian(-45), D3DXToRadian(45), 0);
   lights.insert(light);

   Light* hudLight = factory.createLight("hudLight");
   hudLight->setType(Light::LT_DIRECTIONAL);
   hudLight->setDiffuseColor(Color(1, 1, 1, 1));
   hudLight->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   m_hudLightsScene->insert(hudLight);


   // add a particle system
   LightReflectingProperties* particleLrp = factory.createLightReflectingProperties();
   particleLrp->setDiffuseColor(Color(1, 1, 1, 1));
   MaterialStage* particleMatStage = factory.createMaterialStage("particle.tga",
                                                                 MOP_SELECT_ARG1, SC_LRP, SC_NONE,
                                                                 MOP_MULTIPLY, SC_LRP, SC_TEXTURE,
                                                                 CC_CLAMP);
   Material* particleMat = factory.createMaterial("particleMat", particleLrp);
   particleMat->addStage(particleMatStage);
   m_materialsStorage.add(particleMat);

   m_atmosphere = factory.createParticleSystem("atmosphere", *particleMat, 10000);
   m_atmosphere->setEmissionTime(20);
   m_atmosphere->setLifeSpan(10, 1);
   m_atmosphere->setParticleAnimator(new ParticleFader());
   m_atmosphere->setParticleInitializer(new PlanarParticleInitializer(200, 0.2f, 0.1f, 20));
   D3DXMatrixTranslation(&(m_atmosphere->accessLocalMtx()), 0, -20, 50);
   dynamicScene.addNode(m_atmosphere);
   animationTrack.add(new TTimeDependent<ParticleSystem> (*m_atmosphere));

   // create the cursor
   particleLrp = factory.createLightReflectingProperties();
   particleLrp->setDiffuseColor(Color(1, 0, 1, 1));
   MaterialStage* cursorMatStage = factory.createMaterialStage("particle.tga",
                                                               MOP_SELECT_ARG1, SC_LRP, SC_NONE,
                                                               MOP_ADD_SIGNED, SC_LRP, SC_TEXTURE,
                                                               CC_CLAMP);
   Material* cursorMat = factory.createMaterial("cursorMat", particleLrp);
   cursorMat->addStage(cursorMatStage);
   m_materialsStorage.add(cursorMat);
   m_cursor = factory.createParticleSystem("cursor", *cursorMat, 200);
   m_cursor->setEmissionTime(0.2f);
   m_cursor->setLifeSpan(0.3f, 0.2f);
   m_cursor->setParticleAnimator(new ParticleFader());
   m_cursor->setParticleInitializer(new PointParticleInitializer(0.02f, 0.01f, 0.5f));
   D3DXMatrixTranslation(&(m_cursor->accessLocalMtx()), 0, 0, 10);
   m_hudMeshesScene->addNode(m_cursor);
   animationTrack.add(new TTimeDependent<ParticleSystem> (*m_cursor));

   // create the particel burst that will be activated each time mouse is clicked
   particleLrp = factory.createLightReflectingProperties();
   particleLrp->setDiffuseColor(Color(0, 1, 0.2f, 1));
   MaterialStage* burstMatStage = factory.createMaterialStage("particle.tga",
                                                               MOP_SELECT_ARG1, SC_LRP, SC_NONE,
                                                               MOP_ADD_SIGNED, SC_LRP, SC_TEXTURE,
                                                               CC_CLAMP);
   Material* burstMat = factory.createMaterial("burstMat", particleLrp);
   burstMat->addStage(burstMatStage);
   m_materialsStorage.add(burstMat);
   m_burst = factory.createParticleSystem("burst", *burstMat, 300);
   m_burst->setEmissionTime(0.1f);
   m_burst->setLifeSpan(1.f, 0.2f);
   m_burst->setParticleAnimator(new ParticleFader());
   m_burst->setParticleInitializer(new PointParticleInitializer(0.02f, 0.01f, 0.5f));
   m_burst->setLooped(false);
   m_burst->deactivate();
   D3DXMatrixTranslation(&(m_burst->accessLocalMtx()), 0, 0, 10);
   m_cursor->addChild(m_burst);
   animationTrack.add(new TTimeDependent<ParticleSystem> (*m_burst));

   // add a few objects
   D3DXMATRIX helperMtx;

   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tile1");
   entInstance->attachEntity(*ent);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), 0, 0, 30);
   dynamicScene.addNode(entInstance);
   m_cameraController->registerWaypoint(0, *entInstance);
   m_actionsExecutor->add(*entInstance, new JumpToNodeAction(*m_cameraController, m_shownNode));

   entInstance = new GraphicalEntityInstantiator("tile2");
   entInstance->attachEntity(*ent);
   D3DXMatrixRotationYawPitchRoll(&helperMtx, D3DXToRadian(60), 0, 0);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), 20, 5, 40);
   D3DXMatrixMultiply(&(entInstance->accessLocalMtx()), &helperMtx, &(entInstance->accessLocalMtx()));
   dynamicScene.addNode(entInstance);
   m_cameraController->registerWaypoint(1, *entInstance);
   m_actionsExecutor->add(*entInstance, new JumpToNodeAction(*m_cameraController, m_shownNode));

   entInstance = new GraphicalEntityInstantiator("tile3");
   entInstance->attachEntity(*ent);
   D3DXMatrixRotationYawPitchRoll(&helperMtx, D3DXToRadian(-30), 0, 0);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), -15, -10, 35);
   D3DXMatrixMultiply(&(entInstance->accessLocalMtx()), &helperMtx, &(entInstance->accessLocalMtx()));
   dynamicScene.addNode(entInstance);
   m_cameraController->registerWaypoint(2, *entInstance);
   m_actionsExecutor->add(*entInstance, new JumpToNodeAction(*m_cameraController, m_shownNode));

      // cicular emitter
   particleLrp = factory.createLightReflectingProperties();
   particleLrp->setDiffuseColor(Color(0.2f, 1, 0.2f, 1));
   MaterialStage* circularMatStage = factory.createMaterialStage("particle.tga",
                                                                 MOP_SELECT_ARG1, SC_LRP, SC_NONE,
                                                                 MOP_ADD_SIGNED, SC_LRP, SC_TEXTURE,
                                                                 CC_CLAMP);
   Material* circularMat = factory.createMaterial("circularMat", particleLrp);
   circularMat->addStage(circularMatStage);
   m_materialsStorage.add(circularMat);
   m_circular = factory.createParticleSystem("circular", *circularMat, 100);
   m_circular->setEmissionTime(0.1f);
   m_circular->setLifeSpan(2.f, 0.2f);
   m_circular->setParticleAnimator(new ParticleFader());
   m_circular->setParticleInitializer(new CircularParticleInitializer(5, 0.1f, 0.01f, 10.f));
   D3DXMatrixRotationYawPitchRoll(&helperMtx, 0, D3DXToRadian(45), D3DXToRadian(45));
   D3DXMatrixTranslation(&(m_circular->accessLocalMtx()), 0, 10, 0);
   D3DXMatrixMultiply(&(m_circular->accessLocalMtx()), &helperMtx, &(m_circular->accessLocalMtx()));
   entInstance->addChild(m_circular);
   animationTrack.add(new TTimeDependent<ParticleSystem> (*m_circular));

   m_shownNode = 0;
   m_cameraController->goTo(m_shownNode);

}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::initInput(UserInputController& uiController)
{
   m_inputController = new InputController(uiController, m_tamy.renderer(),
                                          *m_actionsExecutor, *m_burst,
                                          *m_cursor, *m_sceneCamera, *m_mainScene);

   timeController().get("animationTrack").add(new TTimeDependent<InputController> (*m_inputController));
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::onDeinitialize()
{   
   timeController().remove("animationTrack");

   m_hudMeshesScene = NULL;
   m_hudLightsScene = NULL;

   delete m_inputController;
   m_inputController = NULL;

   delete m_circular;
   m_circular = NULL;

   delete m_burst;
   m_burst = NULL;

   delete m_cursor;
   m_cursor = NULL;

   delete m_atmosphere;
   m_atmosphere = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_actionsExecutor;
   m_actionsExecutor = NULL;

   delete m_hudCamera;
   m_hudCamera = NULL;

   delete m_sceneCamera;
   m_sceneCamera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

DEMO(PickingDemo)

///////////////////////////////////////////////////////////////////////////////
