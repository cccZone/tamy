#include "PickingDemo.h"
#include "impl-DirectX\D3DApplicationManager.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-Renderer\Renderer.h"
#include "core\Point.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "core\CompositeSceneManager.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\ParticleSystem.h"
#include "core-Renderer\LinearParticleAnimator.h"
#include "core-Renderer\PlanarParticleInitializer.h"
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
#include "core\dostream.h"


///////////////////////////////////////////////////////////////////////////////

PickingDemo::PickingDemo()
      : Application("Demo"),
      m_renderer(NULL),
      m_resourceManager(NULL),
      m_sceneManager(NULL),
      m_atmosphere(NULL),
      m_cursor(NULL),
      m_actionsExecutor(NULL),
      m_cameraController(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::initialize(Renderer& renderer, ResourceManager& resourceManager)
{
   m_renderer = &renderer;
   m_resourceManager = &resourceManager;

   m_shownNode = 2;
   m_actionsExecutor = new NodeActionsExecutor();

   m_sceneManager = new CompositeSceneManager();
   m_visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(m_visualSceneManager);
   m_renderer->addVisualSceneManager(*m_visualSceneManager);

   m_hudSceneManager = new CompositeSceneManager();
   VisualSceneManager* hudVisualSceneMgr = new VisualSceneManager();
   m_hudSceneManager->addSceneManager(hudVisualSceneMgr);
   m_renderer->addVisualSceneManager(*hudVisualSceneMgr);


   GraphicalEntityLoader& loader =  m_resourceManager->getLoaderForFile("meadowNormalTile.x");
   AbstractGraphicalEntity& ent = m_resourceManager->loadGraphicalEntity("meadowNormalTile.x", loader);

   // add lighting and such
   Light* light = m_resourceManager->createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));
   light->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   D3DXMatrixRotationYawPitchRoll(&(light->accessLocalMtx()), D3DXToRadian(-45), D3DXToRadian(45), 0);
   m_sceneManager->addNode(light);

   Camera* camera = m_resourceManager->createCamera("camera");
   m_sceneManager->addNode(camera);
   m_cameraController = new WaypointCameraController<LinearTimeFunc>(*camera, D3DXVECTOR3(0, 10, -30));

   Camera* hudCamera = m_resourceManager->createCamera("hudCamera");
   hudCamera->setProjectionCalculator(new ProjCalc2D());
   m_hudSceneManager->addNode(hudCamera);

   // add a particle system
   LightReflectingProperties* particleLrp = m_resourceManager->createLightReflectingProperties();
   particleLrp->setDiffuseColor(Color(1, 1, 1, 1));
   particleLrp = &(m_resourceManager->addLightReflectingProperties(particleLrp));

   Texture& tex = m_resourceManager->loadTexture("particle.tga");
   Material& particleMat = m_resourceManager->createMaterial("particleMat", *particleLrp);
   MaterialStage* particleMatStage = m_resourceManager->createMaterialStage(
                                                               tex, 
                                                               MOP_SELECT_ARG1, SC_LRP, SC_NONE,
                                                               MOP_MULTIPLY, SC_LRP, SC_TEXTURE);
   particleMat.addStage(particleMatStage);
   m_atmosphere = m_resourceManager->createParticleSystem("atmosphere", false, particleMat, 200);
   m_atmosphere->setLifeSpan(10, 1);
   m_atmosphere->setParticleAnimator(new LinearParticleAnimator(D3DXVECTOR3(0, 20, 0)));
   m_atmosphere->setParticleInitializer(new PlanarParticleInitializer(200, 0.2f, 0.1f));
   D3DXMatrixTranslation(&(m_atmosphere->accessLocalMtx()), 0, -20, 100);
   m_sceneManager->addNode(m_atmosphere);

   // create the cursor
   particleLrp = m_resourceManager->createLightReflectingProperties();
   particleLrp->setDiffuseColor(Color(1, 0, 1, 1));
   particleLrp = &(m_resourceManager->addLightReflectingProperties(particleLrp));
   Material& cursorMat = m_resourceManager->createMaterial("cursorMat", *particleLrp);
   MaterialStage* cursorMatStage = m_resourceManager->createMaterialStage(
                                                               tex, 
                                                               MOP_SELECT_ARG1, SC_LRP, SC_NONE,
                                                               MOP_ADD_SIGNED, SC_LRP, SC_TEXTURE);
   cursorMat.addStage(cursorMatStage);
   m_cursor = m_resourceManager->createParticleSystem("cursor", true, cursorMat, 200);
   m_cursor->setLifeSpan(1, 0.3f);
   m_cursor->setParticleAnimator(new LinearParticleAnimator(D3DXVECTOR3(0, -0.5, 0)));
   m_cursor->setParticleInitializer(new PlanarParticleInitializer(0.02f, 0.05f, 0.025f));
   D3DXMatrixTranslation(&(m_cursor->accessLocalMtx()), 0, 0, 10);
   m_hudSceneManager->addNode(m_cursor);

   // add a few objects
   D3DXMATRIX helperMtx;

   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tile1", false);
   entInstance->attachEntity(ent);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), 0, 0, 30);
   m_sceneManager->addNode(entInstance);
   m_actionsExecutor->add(*entInstance, NodeActionDelegate::FROM_METHOD(PickingDemo, jumpToNext, this));
   m_cameraController->registerWaypoint(0, *entInstance);

   entInstance = new GraphicalEntityInstantiator("tile2", false);
   entInstance->attachEntity(ent);
   D3DXMatrixRotationYawPitchRoll(&helperMtx, D3DXToRadian(60), 0, 0);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), 20, 5, 40);
   D3DXMatrixMultiply(&(entInstance->accessLocalMtx()), &helperMtx, &(entInstance->accessLocalMtx()));
   m_sceneManager->addNode(entInstance);
   m_actionsExecutor->add(*entInstance, NodeActionDelegate::FROM_METHOD(PickingDemo, jumpToNext, this));
   m_cameraController->registerWaypoint(1, *entInstance);

   entInstance = new GraphicalEntityInstantiator("tile3", false);
   entInstance->attachEntity(ent);
   D3DXMatrixRotationYawPitchRoll(&helperMtx, D3DXToRadian(-30), 0, 0);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), -15, -10, 35);
   D3DXMatrixMultiply(&(entInstance->accessLocalMtx()), &helperMtx, &(entInstance->accessLocalMtx()));
   m_sceneManager->addNode(entInstance);
   m_actionsExecutor->add(*entInstance, NodeActionDelegate::FROM_METHOD(PickingDemo, jumpToNext, this));
   m_cameraController->registerWaypoint(2, *entInstance);

   jumpToNext();
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::deinitialize()
{
   m_cursor = NULL;
   m_atmosphere = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_actionsExecutor;
   m_actionsExecutor = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;

   m_visualSceneManager = NULL;
   m_renderer = NULL;
   m_resourceManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::update(float timeElapsed)
{
   static bool keyPressed = false;
   static bool onPress = false;

   if (context().isKeyPressed(VK_LBUTTON))
   {
      onPress = (keyPressed == false);
      keyPressed = true;
   }
   else if (context().isKeyPressed(VK_LBUTTON) == false)
   {
      keyPressed = false;
   }

   if (onPress)
   {
      Array<Node*> nodes;
      performQuery(nodes);

      m_actionsExecutor->execute(nodes);
   }

   const Point& mouseScreenPos = context().getMousePos();
   D3DXVECTOR2 mouseViewportPos(0, 0);
   m_renderer->screenToViewport(mouseScreenPos, mouseViewportPos);
   D3DXMatrixTranslation(&(m_cursor->accessLocalMtx()), mouseViewportPos.x, mouseViewportPos.y, 10);

   m_cameraController->update(timeElapsed);
   m_atmosphere->update(timeElapsed);
   m_cursor->update(timeElapsed);
   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::performQuery(Array<Node*>& nodes)
{
   // construct the query ray
   Point mousePos = context().getMousePos();

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

void PickingDemo::jumpToNext()
{
   m_shownNode = (m_shownNode + 1) % 3;
   m_cameraController->goTo(m_shownNode);
}


///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   D3DApplicationManager applicationManager(hInstance, nCmdShow, "Picking Demo");
	PickingDemo app;

   applicationManager.addApplication(app);
   applicationManager.setEntryApplication(app.getName());

   while (applicationManager.step()) {}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
