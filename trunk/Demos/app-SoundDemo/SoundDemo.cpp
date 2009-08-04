#include "SoundDemo.h"
#include "tamy\Tamy.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-AppFlow\UserInputController.h"
#include "core-AppFlow\ApplicationManager.h"
#include "core-Renderer\Renderer.h"
#include "core\Point.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Sound\SoundEntitiesFactory.h"
#include "core\CompositeSceneManager.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "core-Renderer\GraphicalEntity.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core-Sound\SoundDevice.h"
#include "core-Sound\SoundChannel.h"
#include "core-Sound\WavFile.h"
#include "core-Sound\SoundListener.h"
#include "core-Sound\SoundRenderer.h"
#include "core-Sound\SoundSceneManager.h"
#include "core-Sound\Sound3D.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\SceneRenderingMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include "tamy\SimpleTamyConfigurator.h"


///////////////////////////////////////////////////////////////////////////////

SoundDemo::SoundDemo(Tamy& tamy)
      : Application("Demo"),
      m_renderer(&(tamy.renderer())),
      m_tamy(tamy),
      m_renderingTarget(NULL),
      m_uiController(m_tamy.uiController())
{
   timeController().add("regularTrack");
   timeController().get("regularTrack").add(new TTimeDependent<SoundDemo>(*this));
}

///////////////////////////////////////////////////////////////////////////////

void SoundDemo::initialize()
{
   m_rotating = false;
   m_sceneManager = new CompositeSceneManager();
   VisualSceneManager* visualSceneManager = new VisualSceneManager();
   m_audioSoundScene = new SoundSceneManager();
   m_sceneManager->addSceneManager(visualSceneManager);
   m_sceneManager->addSceneManager(m_audioSoundScene);

   SettableRenderingTargetsPolicy* sceneRenderingTargetPolicy = new SettableRenderingTargetsPolicy();
   SceneRenderingMechanism* sceneRenderer = m_tamy.graphicalFactory().createSceneRenderingMechanism(sceneRenderingTargetPolicy);
   m_renderer->addMechanism(sceneRenderer);
   m_renderingTarget = m_tamy.graphicalFactory().createDefaultRenderingTarget();
   sceneRenderingTargetPolicy->addTarget(0, *m_renderingTarget);
   sceneRenderer->addVisualSceneManager(*visualSceneManager);

   m_tamy.soundRenderer().addSoundScene(*m_audioSoundScene);

   Light* light = m_tamy.graphicalFactory().createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));
   light->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   D3DXMatrixRotationYawPitchRoll(&(light->accessLocalMtx()), D3DXToRadian(-45), D3DXToRadian(45), 0);
   m_sceneManager->addNode(light);

   // prepare our 'listener'
   Camera* camera = m_tamy.graphicalFactory().createCamera("camera");
   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 0, 10, -20);
   m_cameraController = new UnconstrainedMotionController(*camera);

   m_soundListener = m_tamy.soundFactory().createListener();
   camera->addChild(m_soundListener);
   m_sceneManager->addNode(camera);

   // prepare tiles that emit sounds
   GraphicalEntityLoader loader(m_tamy.graphicalFactory(), m_materialsStorage);
   AbstractGraphicalEntity* ent = loader.load("meadowNormalTile.x", m_tamy.meshLoaders());
   m_entitiesStorage.add(ent);
   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tile", false);
   entInstance->attachEntity(*ent);

   m_sound = new WavFile("..\\Data\\Footsteps.wav");
   Sound3D* tileSound = m_tamy.soundFactory().createEmiter("tileSound", false, *m_sound, 100);
   entInstance->addChild(tileSound);

   m_sceneManager->addNode(entInstance);
}

///////////////////////////////////////////////////////////////////////////////

void SoundDemo::deinitialize()
{
   delete m_renderingTarget;
   m_renderingTarget = NULL;

   delete m_sound;
   m_sound = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SoundDemo::update(float timeElapsed)
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

   m_tamy.soundRenderer().update(timeElapsed);
   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   SimpleTamyConfigurator configurator(1024, 768, false);
   Tamy::initialize(hInstance, nCmdShow, "Sound Demo", configurator);

   // create the application components
	SoundDemo app(TAMY);

   ApplicationManager& appMgr = TAMY.appManager();

   appMgr.addApplication(app);
   appMgr.setEntryApplication(app.getName());

   // run the app
   while (appMgr.step()) {Sleep(0);}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
