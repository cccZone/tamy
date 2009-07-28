#include "TransparencyDemo.h"
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
#include "core-Renderer\Skeleton.h"
#include "ext-MaterialsParser\MaterialsParser.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\SceneRenderingMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"


///////////////////////////////////////////////////////////////////////////////

TransparencyDemo::TransparencyDemo(Tamy& tamy)
      : Application("Demo"),
      m_renderer(&(tamy.renderer())),
      m_tamy(tamy),
      m_renderingTarget(NULL),
      m_uiController(m_tamy.uiController()),
      m_sceneManager(NULL),
      m_cameraController(NULL)
{
   timeController().add("regularTrack");
   timeController().get("regularTrack").add(new TTimeDependent<TransparencyDemo>(*this));
}

///////////////////////////////////////////////////////////////////////////////

void TransparencyDemo::initialize()
{
   m_rotating = false;
   m_sceneManager = new CompositeSceneManager();
   VisualSceneManager* visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(visualSceneManager);
   
   SettableRenderingTargetsPolicy* sceneRenderingTargetPolicy = new SettableRenderingTargetsPolicy();
   SceneRenderingMechanism* sceneRenderer = m_tamy.createSceneRenderingMechanism(sceneRenderingTargetPolicy);
   m_renderer->addMechanism(sceneRenderer);
   m_renderingTarget = m_tamy.graphicalFactory().createDefaultRenderingTarget();
   sceneRenderingTargetPolicy->addTarget(0, *m_renderingTarget);
   sceneRenderer->addVisualSceneManager(*visualSceneManager);

   MaterialsParser parser(m_tamy.graphicalFactory(), m_renderingTechniquesStorage);
   parser.load("..\\Data\\materials.xml");

   IWFLoader loader(m_tamy.graphicalFactory(), 
                    m_tamy.meshLoaders(),
                    *m_sceneManager, 
                    m_entitiesStorage,
                    m_renderingTechniquesStorage);
   loader.load("..\\Data\\Dolphin.iwf");

   Camera* camera = m_tamy.graphicalFactory().createCamera("camera");
   m_sceneManager->addNode(camera);
   m_cameraController = new UnconstrainedMotionController(*camera);
}

///////////////////////////////////////////////////////////////////////////////

void TransparencyDemo::deinitialize()
{
   delete m_renderingTarget;
   m_renderingTarget = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TransparencyDemo::update(float timeElapsed)
{
   m_uiController.update(timeElapsed);
   float movementSpeed = 40 * timeElapsed;
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
   Tamy::initialize(hInstance, nCmdShow, "Transparency Demo", 1024, 768, false);

   // create the application components
	TransparencyDemo app(TAMY);

   ApplicationManager& appMgr = TAMY.appManager();

   appMgr.addApplication(app);
   appMgr.setEntryApplication(app.getName());

   // run the app
   while (appMgr.step()) {Sleep(0);}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
