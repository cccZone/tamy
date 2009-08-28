#include "SkinningDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Skeleton.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

SkinningDemo::SkinningDemo(Tamy& tamy)
: DemoApp(tamy)
{
   timeController().add("animationTrack");
}

///////////////////////////////////////////////////////////////////////////////

void SkinningDemo::initializeScene(demo::DynMeshesScene& dynamicScene, 
                                   demo::LightsScene& lights)
{   
   GraphicalEntitiesFactory& factory = m_tamy.graphicalFactory();
   GraphicalEntityLoader loader(factory, m_materialsStorage);

   AbstractGraphicalEntity* ent = loader.load("US Ranger.x", m_tamy.meshLoaders());
   m_entitiesStorage.add(ent);

   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("ranger01");
   entInstance->attachEntity(*ent);
   D3DXMATRIX rotMtx;
   D3DXMatrixRotationYawPitchRoll(&rotMtx, D3DXToRadian(180), D3DXToRadian(20), 0);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), 0, -20, 50);
   D3DXMatrixMultiply(&(entInstance->accessLocalMtx()), &rotMtx, &(entInstance->accessLocalMtx()));
   dynamicScene.addNode(entInstance);

   m_animationController = ent->instantiateSkeleton(*entInstance);
   m_animationController->activateAnimation("", true);
   timeController().get("animationTrack").add(new TTimeDependent<Skeleton>(*m_animationController));

   Light* light = factory.createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 0));
   light->setLookVec(D3DXVECTOR3(0, 0, 1));
   lights.insert(light);
}

///////////////////////////////////////////////////////////////////////////////

void SkinningDemo::onDeinitialize()
{   
   timeController().remove("animationTrack");

   delete m_animationController;
   m_animationController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

DEMO(SkinningDemo)

///////////////////////////////////////////////////////////////////////////////
