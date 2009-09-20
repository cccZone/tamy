#include "ext-Demo\DemoApp.h"
#include "tamy\Tamy.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-AppFlow\UserInputController.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\SortingRenderablesStorage.h"
#include "core-Renderer\LightsStorage.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\SkyBoxStorage.h"
#include "core-Renderer\TextField.h"
#include "core-Renderer\Font.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "ext-Demo\DemoIWFScene.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\DefaultDemoInputController.h"
#include "ext-Demo\StaticSceneManager.h"
#include "ext-Demo\FpsCounter.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

DemoApp::DemoApp(Tamy& tamy)
: Application("Demo")
, m_tamy(tamy)
, m_renderer(&(tamy.renderer()))
, m_uiController(m_tamy.uiController())
, m_demoController(NULL)
{
   timeController().add("regularTrack");
   timeController().get("regularTrack").add(new TTimeDependent<UserInputController>(m_uiController));

   D3DXFONT_DESC fontDesc = {24,
                          0,
                          400,
                          0,
                          false,
                          DEFAULT_CHARSET,
                          OUT_TT_PRECIS,
                          CLIP_DEFAULT_PRECIS,
                          DEFAULT_PITCH,
                          "Arial"};

   m_font = m_tamy.graphicalFactory().createFont(fontDesc);

   m_fpsView = new TextField(*m_renderer);
   m_fpsView->setFont(*m_font);
   m_fpsView->setSize(0, 0, 0.2f, 0.2f);

   timeController().add("fpsTrack");
   timeController().get("fpsTrack").add(new FpsCounter(*m_fpsView));
}

///////////////////////////////////////////////////////////////////////////////

DemoApp::~DemoApp()
{
   delete m_demoController;
   m_demoController = NULL;

   timeController().remove("fpsTrack");

   delete m_font;
   m_font = NULL;

   delete m_fpsView;
   m_fpsView = NULL;

}

///////////////////////////////////////////////////////////////////////////////

void DemoApp::loadIWF(const std::string& fileName,
                      SkyBoxStorage** outSkyBox,
                      StaticSceneManager** outStaticScene,
                      DynMeshesScene** outDynamicScene, 
                      LightsScene** outLights)
{
   class SkyBoxSetter
   {
   private:
      SkyBoxStorage* m_outSkyBox;

   public:
      SkyBoxSetter() : m_outSkyBox(NULL) {}

      void setBackground(SkyBoxStorage* bg) 
      {
         delete m_outSkyBox;
         m_outSkyBox = bg;
      }

      SkyBoxStorage* getSkyBox() {return m_outSkyBox;}
   };
   SkyBoxSetter skyBoxSetter;

   AABoundingBox worldSize(D3DXVECTOR3(-2000, -2000, -2000), 
                           D3DXVECTOR3(2000, 2000, 2000));
   int treeDepth = 3;
   int maxTreeLeafElems = 64;
   *outStaticScene = new StaticSceneManager(worldSize, maxTreeLeafElems, treeDepth);
   *outDynamicScene = new DynMeshesScene(new LinearStorage<RenderableNode>());
   *outLights = new LightsScene();

   DemoIWFScene sceneAdapter(m_tamy.graphicalFactory(), 
                             m_tamy.meshLoaders(),
                             demo::LightsSceneSetter::from_method<LightsScene, &LightsScene::insert> (*outLights),
                             demo::SkyBoxSceneSetter::from_method<SkyBoxSetter, &SkyBoxSetter::setBackground> (&skyBoxSetter),
                             demo::DynamicObjectsSceneSetter::from_method<demo::DynMeshesScene, &demo::DynMeshesScene::addNode> (*outDynamicScene),
                             demo::StaticGeometrySetter::from_method<StaticSceneManager, &StaticSceneManager::addObj> (*outStaticScene),
                             m_entitiesStorage,
                             m_materialsStorage);
   IWFLoader loader(sceneAdapter);
   loader.load(fileName);

   if (skyBoxSetter.getSkyBox() == NULL)
   {
      GraphicalEntitiesFactory& factory = m_tamy.graphicalFactory();
      *outSkyBox = new SkyBoxStorage(factory.createSkyBoxSide(SBS_LEFT, factory.createTexture("")),
                                     factory.createSkyBoxSide(SBS_RIGHT, factory.createTexture("")),
                                     factory.createSkyBoxSide(SBS_TOP, factory.createTexture("")),
                                     factory.createSkyBoxSide(SBS_BOTTOM, factory.createTexture("")),
                                     factory.createSkyBoxSide(SBS_FRONT, factory.createTexture("")),
                                     factory.createSkyBoxSide(SBS_BACK, factory.createTexture("")));
   }
   else
   {
      *outSkyBox = skyBoxSetter.getSkyBox();
   }
}

///////////////////////////////////////////////////////////////////////////////

Overlay& DemoApp::getFpsView()
{
   return *m_fpsView;
}

///////////////////////////////////////////////////////////////////////////////

ResourceStorage<AbstractGraphicalEntity>& DemoApp::getEntitiesStorage()
{
   return m_entitiesStorage;
}

///////////////////////////////////////////////////////////////////////////////

ResourceStorage<Material>& DemoApp::getMaterialsStorage()
{
   return m_materialsStorage;
}

///////////////////////////////////////////////////////////////////////////////

void DemoApp::createDefaultInput(Camera& camera)
{
   delete m_demoController;
   m_demoController = new DefaultDemoInputController(m_uiController, 
                                                     camera, 
                                                     EndDemoCommand::from_method<DemoApp, &DemoApp::endDemo>(this));
   timeController().remove("uiTrack");
   timeController().add("uiTrack");
   timeController().get("uiTrack").add(new TTimeDependent<DefaultDemoInputController>(*m_demoController));
}

///////////////////////////////////////////////////////////////////////////////

void DemoApp::endDemo()
{
   context().signal(*this, ON_EXIT);
}

///////////////////////////////////////////////////////////////////////////////
