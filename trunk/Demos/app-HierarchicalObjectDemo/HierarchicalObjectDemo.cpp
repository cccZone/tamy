#include "HierarchicalObjectDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\DemoIWFScene.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

HierarchicalObjectDemo::HierarchicalObjectDemo(Tamy& tamy)
: DemoApp(tamy)
{
}

///////////////////////////////////////////////////////////////////////////////

void HierarchicalObjectDemo::initializeScene(demo::DynMeshesScene& dynamicScene, 
                                             demo::LightsScene& lights)
{   
   DemoIWFScene sceneAdapter(m_tamy.graphicalFactory(), 
                             m_tamy.meshLoaders(),
                             demo::LightsSceneSetter::from_method<LightsScene, &LightsScene::insert> (&lights),
                             demo::SkyBoxSceneSetter::from_method<demo::DemoApp, &demo::DemoApp::setBackground> (this),
                             demo::DynamicObjectsSceneSetter::from_method<demo::DynMeshesScene, &demo::DynMeshesScene::addNode> (&dynamicScene),
                             m_entitiesStorage,
                             m_materialsStorage);
   IWFLoader loader(sceneAdapter);
   loader.load("..\\Data\\Space_Scene.iwf");
}

///////////////////////////////////////////////////////////////////////////////

void HierarchicalObjectDemo::onDeinitialize()
{   
}


///////////////////////////////////////////////////////////////////////////////

DEMO(HierarchicalObjectDemo)

///////////////////////////////////////////////////////////////////////////////
