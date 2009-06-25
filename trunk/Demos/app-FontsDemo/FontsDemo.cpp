#include "FontsDemo.h"
#include "impl-DirectX\D3DApplicationManager.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-Renderer\Renderer.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "core\CompositeSceneManager.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\ProjCalc2D.h"
#include "ext-Fonts\VisibleString.h"


///////////////////////////////////////////////////////////////////////////////

FontsDemo::FontsDemo()
      : Application("Demo"),
      m_renderer(NULL),
      m_resourceManager(NULL),
      m_sceneManager(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

void FontsDemo::initialize(ResourceManager& resMgr)
{
   m_renderer = &(resMgr.shared<Renderer>());
   m_resourceManager = &resMgr;

   m_sceneManager = new CompositeSceneManager();
   VisualSceneManager* visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(visualSceneManager);
   m_renderer->addVisualSceneManager(*visualSceneManager);

   Camera* camera = new Camera("camera");
   camera->setProjectionCalculator(new ProjCalc2D());
   m_sceneManager->addNode(camera);

   Light* light = resMgr.resource<Light>()("light");
   m_sceneManager->addNode(light);
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));

   VisibleString* string = new VisibleString(resMgr.resource<Font>()("Curlz.fnt"));
   m_sceneManager->addNode(string);
   D3DXMatrixTranslation(&(string->accessLocalMtx()), -0.5, 0, 2);

   string->setText("Hello World");
}

///////////////////////////////////////////////////////////////////////////////

void FontsDemo::deinitialize()
{
   delete m_sceneManager;
   m_sceneManager = NULL;

   m_renderer = NULL;
   m_resourceManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void FontsDemo::update(float timeElapsed)
{
   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   D3DApplicationManager applicationManager("..\\Data", "..\\Data", "..\\Data",
                                            hInstance, nCmdShow, "Fonts Demo");
	FontsDemo app;

   applicationManager.addApplication(app);
   applicationManager.setEntryApplication(app.getName());

   while (applicationManager.step()) {}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
