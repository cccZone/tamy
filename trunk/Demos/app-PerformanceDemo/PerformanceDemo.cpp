#include "PerformanceDemo.h"
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
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\SceneRenderingMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"


///////////////////////////////////////////////////////////////////////////////

PerformanceDemo::PerformanceDemo(Tamy& tamy)
      : Application("Demo"),
      m_renderer(&(tamy.renderer())),
      m_tamy(tamy),
      m_renderingTarget(NULL),
      m_uiController(m_tamy.uiController()),
      m_sceneManager(NULL),
      m_cameraController(NULL)
{
   timeController().add("regularTrack");
   timeController().get("regularTrack").add(new TTimeDependent<PerformanceDemo>(*this));
}

///////////////////////////////////////////////////////////////////////////////

void PerformanceDemo::initialize()
{
   m_rotating = false;
   m_sceneManager = new CompositeSceneManager();
   m_visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(m_visualSceneManager);

   GraphicalEntitiesFactory& factory = m_tamy.graphicalFactory();

   SettableRenderingTargetsPolicy* sceneRenderingTargetPolicy = new SettableRenderingTargetsPolicy();
   SceneRenderingMechanism* sceneRenderer = m_tamy.graphicalFactory().createSceneRenderingMechanism(sceneRenderingTargetPolicy);
   m_renderer->addMechanism(sceneRenderer);
   m_renderingTarget = factory.createDefaultRenderingTarget();
   sceneRenderingTargetPolicy->addTarget(0, *m_renderingTarget);
   sceneRenderer->addVisualSceneManager(*m_visualSceneManager);

   GraphicalEntityLoader loader(factory, m_renderingTechniquesStorage);

   AbstractGraphicalEntity* ent = loader.load("meadowNormalTile.x", m_tamy.meshLoaders());
   m_entitiesStorage.add(ent);
   for (int y = -20; y < 20; ++y)
   {
      for (int x = -20; x < 20; ++x)
      {
         GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tile", false);
         entInstance->attachEntity(*ent);
         D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), x * 10.f, 0, y * 10.f);

         m_sceneManager->addNode(entInstance);
      }
   }

   Light* light = factory.createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));
   light->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   D3DXMatrixRotationYawPitchRoll(&(light->accessLocalMtx()), D3DXToRadian(-45), D3DXToRadian(45), 0);
   m_sceneManager->addNode(light);

   Camera* camera = m_tamy.graphicalFactory().createCamera("camera");

   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 0, 20, 50);
   m_sceneManager->addNode(camera);
   m_cameraController = new UnconstrainedMotionController(*camera);
}

///////////////////////////////////////////////////////////////////////////////

void PerformanceDemo::deinitialize()
{
   delete m_renderingTarget;
   m_renderingTarget = NULL;

   m_visualSceneManager = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PerformanceDemo::update(float timeElapsed)
{
   m_uiController.update(timeElapsed);
   float movementSpeed = 40.f * timeElapsed;
   float rotationSpeed = 0.1f * timeElapsed;


   // process the keys
   if (m_uiController.isKeyPressed(VK_ESCAPE))     {context().signal(*this, ON_EXIT);}
   if (m_uiController.isKeyPressed(VK_UP))     {m_cameraController->move(m_cameraController->getLookVec()   * movementSpeed);}
   if (m_uiController.isKeyPressed(VK_DOWN))   {m_cameraController->move(-m_cameraController->getLookVec()  * movementSpeed);}
   if (m_uiController.isKeyPressed(VK_LEFT))   {m_cameraController->move(-m_cameraController->getRightVec() * movementSpeed);}
   if (m_uiController.isKeyPressed(VK_RIGHT))  {m_cameraController->move(m_cameraController->getRightVec()  * movementSpeed);}

   if (m_uiController.isKeyPressed(VK_LBUTTON) && (m_rotating == false))
   {
      m_uiController.setRelativeMouseMovement(true);
      m_rotating = true;
   }
   else if ((m_uiController.isKeyPressed(VK_LBUTTON) == false) && m_rotating)
   {
      m_uiController.setRelativeMouseMovement(false);
      m_rotating = false;
   }

   // process the mouse
   if (m_rotating)
   {
      D3DXVECTOR2 mouseSpeed = m_uiController.getMouseSpeed() * rotationSpeed;
      m_cameraController->rotate(mouseSpeed.y, mouseSpeed.x, 0);
   }

   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   Tamy::initialize(hInstance, nCmdShow, "Performance Demo", 1024, 768, false);

   // create the application components
	PerformanceDemo app(TAMY);

   ApplicationManager& appMgr = TAMY.appManager();

   appMgr.addApplication(app);
   appMgr.setEntryApplication(app.getName());

   // run the app
   while (appMgr.step()) {Sleep(0);}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
