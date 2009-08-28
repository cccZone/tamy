#include "TransparencyDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\RenderingPipelineBuilder.h"
#include "core-Renderer\DynamicNodesStorage.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "ext-MaterialsParser\MaterialsParser.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\DemoIWFScene.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

TransparencyDemo::TransparencyDemo(Tamy& tamy)
: DemoApp(tamy)
{
}

///////////////////////////////////////////////////////////////////////////////

void TransparencyDemo::initializeScene(DynMeshesScene& dynamicScene, 
                                       LightsScene& lights)
{   
   MaterialsParser parser(m_tamy.graphicalFactory(), m_materialsStorage);
   parser.load("..\\Data\\materials.xml");

   DemoIWFScene sceneAdapter(m_tamy.graphicalFactory(), 
                             m_tamy.meshLoaders(),
                             demo::LightsSceneSetter::from_method<LightsScene, &LightsScene::insert> (&lights),
                             demo::SkyBoxSceneSetter::from_method<demo::DemoApp, &demo::DemoApp::setBackground> (this),
                             demo::DynamicObjectsSceneSetter::from_method<demo::DynMeshesScene, &demo::DynMeshesScene::addNode> (&dynamicScene),
                             m_entitiesStorage,
                             m_materialsStorage);
   IWFLoader loader(sceneAdapter);
   loader.load("..\\Data\\Dolphin.iwf");
}

///////////////////////////////////////////////////////////////////////////////

void TransparencyDemo::onDeinitialize()
{   
}


///////////////////////////////////////////////////////////////////////////////

DEMO(TransparencyDemo)

///////////////////////////////////////////////////////////////////////////////
