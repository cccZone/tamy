#include "AnimationDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Skeleton.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\DemoIWFScene.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

AnimationDemo::AnimationDemo(Tamy& tamy)
: DemoApp(tamy)
{
   timeController().add("animationTrack");
}

///////////////////////////////////////////////////////////////////////////////

void AnimationDemo::initializeScene(DynMeshesScene& dynamicScene, 
                                    LightsScene& lights)
{   
   DemoIWFScene sceneAdapter(m_tamy.graphicalFactory(), 
                             m_tamy.meshLoaders(),
                             demo::LightsSceneSetter::from_method<LightsScene, &LightsScene::insert> (&lights),
                             demo::SkyBoxSceneSetter::from_method<demo::DemoApp, &demo::DemoApp::setBackground> (this),
                             demo::DynamicObjectsSceneSetter::from_method<demo::DynMeshesScene, &demo::DynMeshesScene::addNode> (&dynamicScene),
                             m_entitiesStorage,
                             m_materialsStorage);
   IWFLoader loader(sceneAdapter);
   loader.load("..\\Data\\AnimLandscape.iwf");

   AbstractGraphicalEntity& ent = m_entitiesStorage.get("animlandscape.x");
   m_animationController = ent.instantiateSkeleton(dynamicScene.root());
   m_animationController->activateAnimation("Cutscene_01", true);
   timeController().get("animationTrack").add(new TTimeDependent<Skeleton>(*m_animationController));

   Light* light = m_tamy.graphicalFactory().createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 0));
   light->setLookVec(D3DXVECTOR3(0, 0, -1));
   lights.insert(light);
}

///////////////////////////////////////////////////////////////////////////////

void AnimationDemo::onDeinitialize()
{   
   timeController().remove("animationTrack");

   delete m_animationController;
   m_animationController = NULL;
}


///////////////////////////////////////////////////////////////////////////////

DEMO(AnimationDemo)

///////////////////////////////////////////////////////////////////////////////
