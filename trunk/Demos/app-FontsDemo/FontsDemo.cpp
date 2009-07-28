#include "FontsDemo.h"
#include "tamy\Tamy.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-AppFlow\ApplicationManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core\CompositeSceneManager.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\ProjCalc2D.h"
#include "core-ResourceManagement\XMLFont.h"
#include "ext-Fonts\VisibleString.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\SceneRenderingMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"


///////////////////////////////////////////////////////////////////////////////

FontsDemo::FontsDemo(Tamy& tamy)
      : Application("Demo"),
      m_renderer(&(tamy.renderer())),
      m_tamy(tamy),
      m_renderingTarget(NULL),
      m_sceneManager2D(NULL),
      m_sceneManager3D(NULL)
{
   timeController().add("regularTrack");
   timeController().get("regularTrack").add(new TTimeDependent<FontsDemo>(*this));
}

///////////////////////////////////////////////////////////////////////////////

void FontsDemo::initialize()
{
   XMLFont* font = new XMLFont("..\\Data\\Curlz.fnt", m_tamy.graphicalFactory());
   m_fontsStorage.add(font);

   SettableRenderingTargetsPolicy* sceneRenderingTargetPolicy = new SettableRenderingTargetsPolicy();
   SceneRenderingMechanism* sceneRenderer = m_tamy.createSceneRenderingMechanism(sceneRenderingTargetPolicy);
   m_renderer->addMechanism(sceneRenderer);
   m_renderingTarget = m_tamy.graphicalFactory().createDefaultRenderingTarget();
   sceneRenderingTargetPolicy->addTarget(0, *m_renderingTarget);


   // scene 2D
   m_sceneManager2D = new CompositeSceneManager();
   VisualSceneManager* visualSceneManager2D = new VisualSceneManager();
   m_sceneManager2D->addSceneManager(visualSceneManager2D);
   sceneRenderer->addVisualSceneManager(*visualSceneManager2D);

   Camera* camera2D = m_tamy.graphicalFactory().createCamera("camera2D");
   camera2D->setProjectionCalculator(new ProjCalc2D());
   m_sceneManager2D->addNode(camera2D);

   Light* light2D = m_tamy.graphicalFactory().createLight("light2D");
   m_sceneManager2D->addNode(light2D);
   light2D->setType(Light::LT_DIRECTIONAL);
   light2D->setDiffuseColor(Color(1, 1, 1, 1));

   VisibleString* string2D = new VisibleString(m_fontsStorage.get("Curlz"));
   m_sceneManager2D->addNode(string2D);
   D3DXMatrixTranslation(&(string2D->accessLocalMtx()), -0.5f, -0.5f, 2);
   string2D->setText("Hello World in 2D");

   // scene 3D
   m_sceneManager3D = new CompositeSceneManager();
   VisualSceneManager* visualSceneManager3D = new VisualSceneManager();
   m_sceneManager3D->addSceneManager(visualSceneManager3D);
   sceneRenderer->addVisualSceneManager(*visualSceneManager3D);

   Camera* camera3D = m_tamy.graphicalFactory().createCamera("camera3D");
   m_sceneManager3D->addNode(camera3D);

   Light* light3D = m_tamy.graphicalFactory().createLight("light3D");
   m_sceneManager3D->addNode(light3D);
   light3D->setType(Light::LT_DIRECTIONAL);
   light3D->setDiffuseColor(Color(1, 1, 1, 1));

   VisibleString* string3D = new VisibleString(m_fontsStorage.get("Curlz"));
   m_sceneManager3D->addNode(string3D);
   D3DXMatrixTranslation(&(string3D->accessLocalMtx()), 0, 0, 80);
   D3DXMATRIX helperMtx;
   D3DXMatrixScaling(&helperMtx, 30, 30, 30);
   D3DXMatrixMultiply(&(string3D->accessLocalMtx()), &helperMtx, &(string3D->accessLocalMtx()));
   string3D->setText("Hello World in 3D");
   string3D->setColor(Color(1, 0.5f, 0.5f, 1));
}

///////////////////////////////////////////////////////////////////////////////

void FontsDemo::deinitialize()
{
   delete m_renderingTarget;
   m_renderingTarget = NULL;

   delete m_sceneManager3D;
   m_sceneManager3D = NULL;

   delete m_sceneManager2D;
   m_sceneManager2D = NULL;
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
   Tamy::initialize(hInstance, nCmdShow, "Fonts Demo", 1024, 768, false);

   // create the application components
	FontsDemo app(TAMY);

   ApplicationManager& appMgr = TAMY.appManager();

   appMgr.addApplication(app);
   appMgr.setEntryApplication(app.getName());

   // run the app
   while (appMgr.step()) {Sleep(0);}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
