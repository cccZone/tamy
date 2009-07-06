#include "FontsDemo.h"
#include "impl-DirectX\Tamy.h"
#include "impl-DirectX\D3DApplicationManager.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core\CompositeSceneManager.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\ProjCalc2D.h"
#include "core-ResourceManagement\XMLFont.h"
#include "ext-Fonts\VisibleString.h"


///////////////////////////////////////////////////////////////////////////////

FontsDemo::FontsDemo(Tamy& tamy)
      : Application("Demo"),
      m_renderer(&(tamy.renderer())),
      m_tamy(tamy),
      m_sceneManager(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

void FontsDemo::initialize()
{
   m_sceneManager = new CompositeSceneManager();
   VisualSceneManager* visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(visualSceneManager);
   m_renderer->addVisualSceneManager(*visualSceneManager);

   Camera* camera = new Camera("camera");
   camera->setProjectionCalculator(new ProjCalc2D());
   m_sceneManager->addNode(camera);

   Light* light = m_tamy.graphicalFactory().createLight("light");
   m_sceneManager->addNode(light);
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));

   XMLFont* font = new XMLFont("..\\Data\\Curlz.fnt", m_tamy.graphicalFactory());
   m_tamy.fontsStorage().add(font);

   VisibleString* string = new VisibleString(m_tamy.fontsStorage().get("Curlz"));
   m_sceneManager->addNode(string);
   D3DXMatrixTranslation(&(string->accessLocalMtx()), -0.5f, 0, 2);

   string->setText("Hello World");
}

///////////////////////////////////////////////////////////////////////////////

void FontsDemo::deinitialize()
{
   delete m_sceneManager;
   m_sceneManager = NULL;
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
   Tamy tamy("..\\Data", "..\\Data", "..\\Data");
   D3DApplicationManager applicationManager(hInstance, nCmdShow, "Fonts Demo", tamy);
	FontsDemo app(tamy);

   applicationManager.addApplication(app);
   applicationManager.setEntryApplication(app.getName());

   while (applicationManager.step()) {}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
