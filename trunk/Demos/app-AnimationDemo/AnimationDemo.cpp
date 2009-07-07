#include "AnimationDemo.h"
#include "impl-DirectX\D3DApplicationManager.h"
#include "impl-DirectX\Tamy.h"
#include "core-AppFlow\ExecutionContext.h"
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
#include "ext-MotionControllers\UnconstrainedMotionController.h"


///////////////////////////////////////////////////////////////////////////////

AnimationDemo::AnimationDemo(Tamy& tamy)
      : Application("Demo"),
      m_tamy(tamy),
      m_renderer(&(tamy.renderer())),
      m_sceneManager(NULL),
      m_cameraController(NULL)
{
   timeController().add("regularTrack");
   timeController().get("regularTrack").add(new TTimeDependent<AnimationDemo>(*this));
}

///////////////////////////////////////////////////////////////////////////////

void AnimationDemo::initialize()
{
   m_rotating = false;
   m_sceneManager = new CompositeSceneManager();
   VisualSceneManager* visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(visualSceneManager);
   m_renderer->addVisualSceneManager(*visualSceneManager);

   IWFLoader loader(m_tamy.graphicalFactory(), 
                    m_tamy.meshLoaders(),
                    *m_sceneManager, 
                    m_entitiesStorage,
                    m_materialsStorage);
   loader.load("..\\Data\\AnimLandscape.iwf");

   AbstractGraphicalEntity& ent = m_entitiesStorage.get("animlandscape.x");
   m_animationController = ent.instantiateSkeleton(m_sceneManager->root());
   m_animationController->activateAnimation("Cutscene_01", true);

   Light* light = m_tamy.graphicalFactory().createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 0));
   light->setLookVec(D3DXVECTOR3(0, 0, -1));
   m_sceneManager->addNode(light);

   Camera* camera = new Camera("camera");
   m_sceneManager->addNode(camera);
   m_cameraController = new UnconstrainedMotionController(*camera);
}

///////////////////////////////////////////////////////////////////////////////

void AnimationDemo::deinitialize()
{
   delete m_animationController;
   m_animationController = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void AnimationDemo::update(float timeElapsed)
{
   float movementSpeed = 40 * timeElapsed;
   float rotationSpeed = 180 * timeElapsed;


   // process the keys
   if (context().isKeyPressed(VK_ESCAPE))     {context().signal(*this, ON_EXIT);}
   if (context().isKeyPressed(VK_UP))     {m_cameraController->move(m_cameraController->getLookVec()   * movementSpeed);}
   if (context().isKeyPressed(VK_DOWN))   {m_cameraController->move(-m_cameraController->getLookVec()  * movementSpeed);}
   if (context().isKeyPressed(VK_LEFT))   {m_cameraController->move(-m_cameraController->getRightVec() * movementSpeed);}
   if (context().isKeyPressed(VK_RIGHT))  {m_cameraController->move(m_cameraController->getRightVec()  * movementSpeed);}

   if (context().isKeyPressed(VK_LBUTTON) && (m_rotating == false))
   {
      context().relativeMouseMovement(true);
      m_rotating = true;
   }
   else if ((context().isKeyPressed(VK_LBUTTON) == false) && m_rotating)
   {
      context().relativeMouseMovement(false);
      m_rotating = false;
   }

   // process the mouse
   if (m_rotating)
   {
      Point mouseRel = context().getMousePos();
      float rotX = (float)(mouseRel.x) / 3.0f;
      float rotY = (float)(mouseRel.y) / 3.0f;
      m_cameraController->rotate(rotY * rotationSpeed, rotX * rotationSpeed, 0);
   }

   m_animationController->update(timeElapsed);
   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   Tamy tamy("..\\Data", "..\\Data", "..\\Data");
   D3DApplicationManager applicationManager(hInstance, nCmdShow, "Animation Demo", tamy);
	AnimationDemo app(tamy);

   applicationManager.addApplication(app);
   applicationManager.setEntryApplication(app.getName());

   while (applicationManager.step()) {}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
