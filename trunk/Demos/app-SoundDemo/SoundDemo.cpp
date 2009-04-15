#include "SoundDemo.h"
#include "impl-DirectX\D3DApplicationManager.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-Renderer\Renderer.h"
#include "core\Point.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "core-Renderer\BasicSceneManager.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core-Sound\SoundDevice.h"
#include "core-Sound\SoundChannel.h"
#include "core-Sound\WavFile.h"
#include "core-Sound\SoundListener.h"
#include "core-Sound\SoundRenderer.h"
#include "core-Sound\BasicSoundSceneManager.h"
#include "core-Sound\Sound3D.h"


///////////////////////////////////////////////////////////////////////////////

SoundDemo::SoundDemo()
      : Application("Demo"),
      m_renderer(NULL),
      m_resourceManager(NULL),
      m_sceneManager(NULL),
      m_cameraController(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

void SoundDemo::initialize(Renderer& renderer, ResourceManager& resourceManager)
{
   m_renderer = &renderer;
   m_resourceManager = &resourceManager;

   m_rotating = false;
   m_sceneManager = new BasicSceneManager();

   m_renderer->addSceneManager(*m_sceneManager);

   GraphicalEntityLoader& loader =  m_resourceManager->getLoaderForFile("meadowNormalTile.x");
   AbstractGraphicalEntity& ent = m_resourceManager->loadGraphicalEntity("meadowNormalTile.x", loader);

   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tile");
   entInstance->attachEntity(ent);

   m_sceneManager->addNode(entInstance);


   Light* light = m_resourceManager->createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));
   light->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   D3DXMatrixRotationYawPitchRoll(&(light->accessLocalMtx()), D3DXToRadian(-45), D3DXToRadian(45), 0);
   m_sceneManager->addNode(light);

   Camera* camera = m_resourceManager->createCamera("camera");

   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 0, 10, -20);
   m_sceneManager->addNode(camera);
   m_sceneManager->setActiveCamera(*camera);
   m_cameraController = new UnconstrainedMotionController(*camera);

   // prepare the sound env
   m_soundRenderer = &(m_resourceManager->getSoundRenderer());

   m_soundListener = m_resourceManager->createSoundListener();
   camera->addChild(m_soundListener);

   m_soundScene = new BasicSoundSceneManager();
   m_soundScene->setActiveListener(*m_soundListener);

    // prepare sounds
   m_sound = new WavFile("..\\Data\\Footsteps.wav");
   Sound3D* tileSound = m_resourceManager->createSound3D("tileSound", *m_sound, 100);
   entInstance->addChild(tileSound);
   m_soundScene->add(*tileSound);
}

///////////////////////////////////////////////////////////////////////////////

void SoundDemo::deinitialize()
{
   delete m_soundScene;
   m_soundScene = NULL;

   delete m_sound;
   m_sound = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;

   m_soundRenderer = NULL;
   m_renderer = NULL;
   m_resourceManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SoundDemo::update(float timeElapsed)
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

   m_soundRenderer->render(*m_soundScene);
   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   D3DApplicationManager applicationManager(hInstance, nCmdShow, "Performance Demo");
	SoundDemo app;

   applicationManager.addApplication(app);
   applicationManager.setEntryApplication(app.getName());

   while (applicationManager.step()) {}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
