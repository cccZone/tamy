#include "PickingDemo.h"
#include "tamy\Tamy.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-AppFlow\UserInputController.h"
#include "core-AppFlow\ApplicationManager.h"
#include "core-Renderer\Renderer.h"
#include "core\Point.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core\CompositeSceneManager.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\ParticleSystem.h"
#include "core-Renderer\ParticleFader.h"
#include "core-Renderer\PlanarParticleInitializer.h"
#include "core-Renderer\PointParticleInitializer.h"
#include "core-Renderer\CircularParticleInitializer.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\ProjCalc2D.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core\Ray.h"
#include "core\MatrixWriter.h"
#include "core\NodeActionsExecutor.h"
#include "ext-MotionControllers\WaypointCameraController.h"
#include "ext-MotionControllers\TimeFunction.h"
#include "core\dostream.h"
#include "JumpToNodeAction.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\SceneRenderingMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"


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
      : Application("Demo"),
      m_renderer(&(tamy.renderer())),
      m_tamy(tamy),
      m_renderingTarget(NULL),
      m_uiController(m_tamy.uiController()),
      m_sceneManager(NULL),
      m_atmosphere(NULL),
      m_cursor(NULL),
      m_burst(NULL),
      m_actionsExecutor(NULL),
      m_cameraController(NULL),
      m_shownNode(0)
{
   timeController().add("regularTrack");
   timeController().get("regularTrack").add(new TTimeDependent<PickingDemo>(*this));
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::initialize()
{
   m_actionsExecutor = new NodeActionsExecutor();

   SettableRenderingTargetsPolicy* sceneRenderingTargetPolicy = new SettableRenderingTargetsPolicy();
   SceneRenderingMechanism* sceneRenderer = m_tamy.createSceneRenderingMechanism(sceneRenderingTargetPolicy);
   m_renderer->addMechanism(sceneRenderer);
   m_renderingTarget = m_tamy.graphicalFactory().createDefaultRenderingTarget();
   sceneRenderingTargetPolicy->addTarget(0, *m_renderingTarget);

   m_sceneManager = new CompositeSceneManager();
   m_visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(m_visualSceneManager);
   sceneRenderer->addVisualSceneManager(*m_visualSceneManager);

   m_hudSceneManager = new CompositeSceneManager();
   VisualSceneManager* hudVisualSceneMgr = new VisualSceneManager();
   m_hudSceneManager->addSceneManager(hudVisualSceneMgr);
   sceneRenderer->addVisualSceneManager(*hudVisualSceneMgr);

   GraphicalEntitiesFactory& factory = m_tamy.graphicalFactory();

   GraphicalEntityLoader loader(factory, m_renderingTechniquesStorage);

   AbstractGraphicalEntity* ent = loader.load("meadowNormalTile.x", m_tamy.meshLoaders());
   m_entitiesStorage.add(ent);

   // add lighting and such
   Light* light = factory.createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));
   light->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   D3DXMatrixRotationYawPitchRoll(&(light->accessLocalMtx()), D3DXToRadian(-45), D3DXToRadian(45), 0);
   m_sceneManager->addNode(light);

   Camera* camera = m_tamy.graphicalFactory().createCamera("camera");
   m_sceneManager->addNode(camera);
   m_cameraController = new WaypointCameraController(*camera, D3DXVECTOR3(0, 10, -30), new LinearTimeFunc());

   Light* hudLight = factory.createLight("hudLight");
   hudLight->setType(Light::LT_DIRECTIONAL);
   hudLight->setDiffuseColor(Color(1, 1, 1, 1));
   hudLight->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   m_hudSceneManager->addNode(hudLight);

   Camera* hudCamera = m_tamy.graphicalFactory().createCamera("hudCamera");
   hudCamera->setProjectionCalculator(new ProjCalc2D());
   m_hudSceneManager->addNode(hudCamera);

   // add a particle system
   LightReflectingProperties* particleLrp = factory.createLightReflectingProperties();
   particleLrp->setDiffuseColor(Color(1, 1, 1, 1));
   MaterialStage* particleMatStage = factory.createMaterialStage("particle.tga",
                                                                 MOP_SELECT_ARG1, SC_LRP, SC_NONE,
                                                                 MOP_MULTIPLY, SC_LRP, SC_TEXTURE,
                                                                 CC_CLAMP);
   Material* particleMat = factory.createMaterial("particleMat", particleLrp);
   particleMat->addStage(particleMatStage);
   m_renderingTechniquesStorage.add(particleMat);

   m_atmosphere = factory.createParticleSystem("atmosphere", false, *particleMat, 10000);
   m_atmosphere->setEmissionTime(20);
   m_atmosphere->setLifeSpan(10, 1);
   m_atmosphere->setParticleAnimator(new ParticleFader());
   m_atmosphere->setParticleInitializer(new PlanarParticleInitializer(200, 0.2f, 0.1f, 20));
   D3DXMatrixTranslation(&(m_atmosphere->accessLocalMtx()), 0, -20, 50);
   m_sceneManager->addNode(m_atmosphere);

   // create the cursor
   particleLrp = factory.createLightReflectingProperties();
   particleLrp->setDiffuseColor(Color(1, 0, 1, 1));
   MaterialStage* cursorMatStage = factory.createMaterialStage("particle.tga",
                                                               MOP_SELECT_ARG1, SC_LRP, SC_NONE,
                                                               MOP_ADD_SIGNED, SC_LRP, SC_TEXTURE,
                                                               CC_CLAMP);
   Material* cursorMat = factory.createMaterial("cursorMat", particleLrp);
   cursorMat->addStage(cursorMatStage);
   m_renderingTechniquesStorage.add(cursorMat);
   m_cursor = factory.createParticleSystem("cursor", true, *cursorMat, 200);
   m_cursor->setEmissionTime(0.2f);
   m_cursor->setLifeSpan(0.3f, 0.2f);
   m_cursor->setParticleAnimator(new ParticleFader());
   m_cursor->setParticleInitializer(new PointParticleInitializer(0.02f, 0.01f, 0.5f));
   D3DXMatrixTranslation(&(m_cursor->accessLocalMtx()), 0, 0, 10);
   m_hudSceneManager->addNode(m_cursor);

   // create the particel burst that will be activated each time mouse is clicked
   particleLrp = factory.createLightReflectingProperties();
   particleLrp->setDiffuseColor(Color(0, 1, 0.2f, 1));
   MaterialStage* burstMatStage = factory.createMaterialStage("particle.tga",
                                                               MOP_SELECT_ARG1, SC_LRP, SC_NONE,
                                                               MOP_ADD_SIGNED, SC_LRP, SC_TEXTURE,
                                                               CC_CLAMP);
   Material* burstMat = factory.createMaterial("burstMat", particleLrp);
   burstMat->addStage(burstMatStage);
   m_renderingTechniquesStorage.add(burstMat);
   m_burst = factory.createParticleSystem("burst", true, *burstMat, 300);
   m_burst->setEmissionTime(0.1f);
   m_burst->setLifeSpan(1.f, 0.2f);
   m_burst->setParticleAnimator(new ParticleFader());
   m_burst->setParticleInitializer(new PointParticleInitializer(0.02f, 0.01f, 0.5f));
   m_burst->setLooped(false);
   m_burst->deactivate();
   D3DXMatrixTranslation(&(m_burst->accessLocalMtx()), 0, 0, 10);
   m_cursor->addChild(m_burst);

   // add a few objects
   D3DXMATRIX helperMtx;

   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tile1", false);
   entInstance->attachEntity(*ent);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), 0, 0, 30);
   m_sceneManager->addNode(entInstance);
   m_cameraController->registerWaypoint(0, *entInstance);
   m_actionsExecutor->add(*entInstance, new JumpToNodeAction(*m_cameraController, m_shownNode));

   entInstance = new GraphicalEntityInstantiator("tile2", false);
   entInstance->attachEntity(*ent);
   D3DXMatrixRotationYawPitchRoll(&helperMtx, D3DXToRadian(60), 0, 0);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), 20, 5, 40);
   D3DXMatrixMultiply(&(entInstance->accessLocalMtx()), &helperMtx, &(entInstance->accessLocalMtx()));
   m_sceneManager->addNode(entInstance);
   m_cameraController->registerWaypoint(1, *entInstance);
   m_actionsExecutor->add(*entInstance, new JumpToNodeAction(*m_cameraController, m_shownNode));

   entInstance = new GraphicalEntityInstantiator("tile3", false);
   entInstance->attachEntity(*ent);
   D3DXMatrixRotationYawPitchRoll(&helperMtx, D3DXToRadian(-30), 0, 0);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), -15, -10, 35);
   D3DXMatrixMultiply(&(entInstance->accessLocalMtx()), &helperMtx, &(entInstance->accessLocalMtx()));
   m_sceneManager->addNode(entInstance);
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
   m_renderingTechniquesStorage.add(circularMat);
   m_circular = factory.createParticleSystem("circular", false, *circularMat, 100);
   m_circular->setEmissionTime(0.1f);
   m_circular->setLifeSpan(2.f, 0.2f);
   m_circular->setParticleAnimator(new ParticleFader());
   m_circular->setParticleInitializer(new CircularParticleInitializer(5, 0.1f, 0.01f, 10.f));
   D3DXMatrixRotationYawPitchRoll(&helperMtx, 0, D3DXToRadian(45), D3DXToRadian(45));
   D3DXMatrixTranslation(&(m_circular->accessLocalMtx()), 0, 10, 0);
   D3DXMatrixMultiply(&(m_circular->accessLocalMtx()), &helperMtx, &(m_circular->accessLocalMtx()));
   entInstance->addChild(m_circular);

   m_shownNode = 0;
   m_cameraController->goTo(m_shownNode);
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::deinitialize()
{
   delete m_renderingTarget;
   m_renderingTarget = NULL;

   m_circular = NULL;
   m_burst = NULL;
   m_cursor = NULL;
   m_atmosphere = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_actionsExecutor;
   m_actionsExecutor = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;

   m_visualSceneManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::update(float timeElapsed)
{
   m_uiController.update(timeElapsed);
   static bool keyPressed = false;
   static bool onPress = false;

   if (m_uiController.isKeyPressed(VK_LBUTTON))
   {
      onPress = (keyPressed == false);
      keyPressed = true;
   }
   else if (m_uiController.isKeyPressed(VK_LBUTTON) == false)
   {
      keyPressed = false;
   }

   if (onPress)
   {
      Array<Node*> nodes;
      performQuery(nodes);

      m_actionsExecutor->execute(nodes);
      m_burst->activate();
   }

   const Point& mouseScreenPos = m_uiController.getMousePos();
   D3DXVECTOR2 mouseViewportPos(0, 0);
   m_renderer->screenToViewport(mouseScreenPos, mouseViewportPos);
   D3DXMatrixTranslation(&(m_cursor->accessLocalMtx()), mouseViewportPos.x, mouseViewportPos.y, 10);

   m_cameraController->update(timeElapsed);
   m_atmosphere->update(timeElapsed);
   m_cursor->update(timeElapsed);
   m_burst->update(timeElapsed);
   m_circular->update(timeElapsed);
   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::performQuery(Array<Node*>& nodes)
{
   // construct the query ray
   Point mousePos = m_uiController.getMousePos();

   Camera& camera = m_visualSceneManager->getActiveCamera();
   D3DXVECTOR2 viewportPos;
   m_renderer->screenToViewport(mousePos, viewportPos);

   Ray queryRay = camera.createRay(viewportPos.x, viewportPos.y);

   // perform the query
   Array<AbstractGraphicalNode*> queriedNodes;
   m_visualSceneManager->detailedQuery<AbstractGraphicalNode>(queryRay, queriedNodes);

   // output the results
   for (unsigned int i = 0; i < queriedNodes.size(); ++i)
   {
      nodes.push_back(queriedNodes[i]);
   }
}

///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   Tamy::initialize(hInstance, nCmdShow, "Picking Demo", 1024, 768, false);

   // create the application components
	PickingDemo app(TAMY);

   ApplicationManager& appMgr = TAMY.appManager();

   appMgr.addApplication(app);
   appMgr.setEntryApplication(app.getName());

   // run the app
   while (appMgr.step()) {Sleep(0);}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
