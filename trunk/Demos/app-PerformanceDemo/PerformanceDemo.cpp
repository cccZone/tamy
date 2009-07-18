#include "PerformanceDemo.h"
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
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core-Renderer\RenderingTarget.h"


///////////////////////////////////////////////////////////////////////////////

PerformanceDemo::PerformanceDemo(Tamy& tamy)
      : Application("Demo"),
      m_renderer(&(tamy.renderer())),
      m_tamy(tamy),
      m_renderingTarget(NULL),
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
   m_renderer->addVisualSceneManager(*m_visualSceneManager);

   GraphicalEntitiesFactory& factory = m_tamy.graphicalFactory();

   m_renderingTarget = factory.createDefaultRenderingTarget();
   m_renderer->addRenderingTarget(*m_renderingTarget);

   GraphicalEntityLoader loader(factory, m_materialsStorage);

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

   Camera* camera = new Camera("camera");

   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 0, 20, 50);
   m_sceneManager->addNode(camera);
   m_cameraController = new UnconstrainedMotionController(*camera);
}

///////////////////////////////////////////////////////////////////////////////

void PerformanceDemo::deinitialize()
{
   delete m_renderingTarget;
   m_renderingTarget = NULL;

   m_renderer->removeVisualSceneManager(*m_visualSceneManager);
   m_visualSceneManager = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PerformanceDemo::update(float timeElapsed)
{
   float movementSpeed = 40.f * timeElapsed;
   float rotationSpeed = 180.f * timeElapsed;


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

   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   Tamy tamy("..\\Data", "..\\Data", "..\\Data");
   D3DApplicationManager applicationManager(hInstance, nCmdShow, "Performance Demo", tamy);
	PerformanceDemo app(tamy);

   applicationManager.addApplication(app);
   applicationManager.setEntryApplication(app.getName());

   while (applicationManager.step()) {}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
