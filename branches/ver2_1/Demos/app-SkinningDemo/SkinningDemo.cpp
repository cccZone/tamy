#include "SkinningDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Skeleton.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-Demo\BasicRenderingPipeline.h"
#include "ext-Demo\SharedOverlay.h"
#include "ext-Demo\LightsScene.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

SkinningDemo::SkinningDemo(Tamy& tamy)
: DemoApp(tamy)
, m_camera(NULL)
, m_renderingPipeline(NULL)
{
   timeController().add("animationTrack");

   m_camera = m_tamy.graphicalFactory().createCamera("camera");
   m_renderingPipeline = new BasicRenderingPipeline(m_tamy.graphicalFactory(), m_tamy.renderer(), *m_camera);
   timeController().add("rendererTrack");
   timeController().get("rendererTrack").add(new TTimeDependent<BasicRenderingPipeline> (*m_renderingPipeline));
}

///////////////////////////////////////////////////////////////////////////////

void SkinningDemo::initialize()
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

   // populate the scene
   demo::DynMeshesScene* dynamicScene = new demo::DynMeshesScene(new LinearStorage<RenderableNode>());
   dynamicScene->addNode(entInstance);

   // create an animation
   m_animationController = ent->instantiateSkeleton(*entInstance);
   m_animationController->activateAnimation("", true);
   timeController().get("animationTrack").add(new TTimeDependent<Skeleton>(*m_animationController));

   Light* light = factory.createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 0));
   light->setLookVec(D3DXVECTOR3(0, 0, 1));
   demo::LightsScene* lights = new demo::LightsScene();
   lights->insert(light);

   // assemble rendering pipeline
   m_renderingPipeline->setDynamicScene(dynamicScene);
   m_renderingPipeline->setLights(lights);
   m_renderingPipeline->setOverlay(new demo::SharedOverlay(getFpsView()));
   m_renderingPipeline->create();

   // initialize input controller
   createDefaultInput(*m_camera);
}

///////////////////////////////////////////////////////////////////////////////

void SkinningDemo::deinitialize()
{   
   timeController().remove("rendererTrack");
   timeController().remove("animationTrack");

   delete m_camera;
   m_camera = NULL;

   delete m_renderingPipeline;
   m_renderingPipeline = NULL;

   delete m_animationController;
   m_animationController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

DEMO(SkinningDemo)

///////////////////////////////////////////////////////////////////////////////
