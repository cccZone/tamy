#include "PickingDemo.h"
#include "impl-DirectX\D3DApplicationManager.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-Renderer\Renderer.h"
#include "core\Point.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "core\CompositeSceneManager.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core\Ray.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

PickingDemo::PickingDemo()
      : Application("Demo"),
      m_renderer(NULL),
      m_resourceManager(NULL),
      m_sceneManager(NULL),
      m_action(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::initialize(Renderer& renderer, ResourceManager& resourceManager)
{
   m_renderer = &renderer;
   m_resourceManager = &resourceManager;

   m_rotating = false;
   m_sceneManager = new CompositeSceneManager();
   m_visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(m_visualSceneManager);
   m_renderer->addVisualSceneManager(*m_visualSceneManager);

   GraphicalEntityLoader& loader =  m_resourceManager->getLoaderForFile("meadowNormalTile.x");
   AbstractGraphicalEntity& ent = m_resourceManager->loadGraphicalEntity("meadowNormalTile.x", loader);

   // add a few objects
   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tile1", false);
   entInstance->attachEntity(ent);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), 0, 0, 30);
   m_sceneManager->addNode(entInstance);
/*
   entInstance = new GraphicalEntityInstantiator("tile2", false);
   entInstance->attachEntity(ent);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), 10, 5, 40);
   m_sceneManager->addNode(entInstance);


   entInstance = new GraphicalEntityInstantiator("tile3", false);
   entInstance->attachEntity(ent);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), -15, -10, 35);
   m_sceneManager->addNode(entInstance);*/

   // add lighting and such
   Light* light = m_resourceManager->createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));
   light->setSpecularColor(Color(0.2, 0.2, 0.2, 1));
   D3DXMatrixRotationYawPitchRoll(&(light->accessLocalMtx()), D3DXToRadian(-45), D3DXToRadian(45), 0);
   m_sceneManager->addNode(light);

   Camera* camera = m_resourceManager->createCamera("camera");
   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 0, 0, 0);
   m_sceneManager->addNode(camera);

   // prepare the action
   QueryNodesAction* queryNodesAction = new QueryNodesAction(renderer, *m_visualSceneManager, context());
   m_action = new IntervalOperation<QueryNodesAction>(0.25f, queryNodesAction);
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::deinitialize()
{
   delete m_action;
   m_action = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;

   m_visualSceneManager = NULL;
   m_renderer = NULL;
   m_resourceManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PickingDemo::update(float timeElapsed)
{
   (*m_action)(timeElapsed);
   m_renderer->render();
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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void QueryNodesAction::operator()()
{
   // construct the query ray
   Point mousePos = m_context.getMousePos();

   Camera& camera = m_sceneMgr.getActiveCamera();
   D3DXVECTOR2 viewportPos;
   m_renderer.screenToViewport(mousePos, viewportPos);

   Ray queryRay = camera.createRay(viewportPos.x, viewportPos.y);

   m_dbg << "query: " << queryRay.direction << std::endl;

   // perform the query
   Array<AbstractGraphicalNode*> nodes;
   m_sceneMgr.detailedQuery<AbstractGraphicalNode>(queryRay, nodes);

   // output the results
   for (unsigned int i = 0; i < nodes.size(); ++i)
   {
      m_dbg << nodes[i]->getName() << std::endl;
   }
   m_dbg << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
