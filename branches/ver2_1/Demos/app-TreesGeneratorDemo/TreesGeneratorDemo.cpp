#include "TreesGeneratorDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Skeleton.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\BasicRenderingPipeline.h"
#include "ext-Demo\SharedOverlay.h"
#include "ext-TreesGenerator\TreeParams.h"
#include "ext-TreesGenerator\TreesGenerator.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

TreesGeneratorDemo::TreesGeneratorDemo(Tamy& tamy)
: DemoApp(tamy)
, m_camera(NULL)
, m_renderingPipeline(NULL)
, m_treeEntity(NULL)
{
   timeController().add("animationTrack");

   m_camera = m_tamy.graphicalFactory().createCamera("camera");
   m_renderingPipeline = new BasicRenderingPipeline(m_tamy.graphicalFactory(), m_tamy.renderer(), *m_camera);
   timeController().add("rendererTrack");
   timeController().get("rendererTrack").add(new TTimeDependent<BasicRenderingPipeline> (*m_renderingPipeline));
}

///////////////////////////////////////////////////////////////////////////////

void TreesGeneratorDemo::initialize()
{   
   TreeParams treeParams;
   treeParams.maxTreeDepth = 5;
   treeParams.startWidth = 10;
   treeParams.startDirection = D3DXVECTOR3(0, 1, 0);
   treeParams.segmentInitialLength = 20;
   treeParams.minInternalBranchDirVariation = 2;
   treeParams.maxInternalBranchDirVariation = 10;
   treeParams.branchProbab.push_back(80);
   treeParams.branchProbab.push_back(30);
   treeParams.minNewBranchDirVariation = 30;
   treeParams.maxNewBranchDirVariation = 60;
   treeParams.initialBranchingLevel = 1;
   treeParams.abruptBranchEndProbab = 5;

   // prepare material
   LightReflectingProperties* lrp = m_tamy.graphicalFactory().createLightReflectingProperties();
   lrp->setAmbientColor(Color(1, 1, 1, 1));
   lrp->setDiffuseColor(Color(1, 1, 1, 1));
   MaterialStage* stage = m_tamy.graphicalFactory().createMaterialStage("LondonPlaneBark.dds",
                                             MOP_MULTIPLY, SC_LRP, SC_TEXTURE,
                                             MOP_DISABLE, SC_NONE, SC_NONE,
                                             CC_WRAP);
   Material* mat = m_tamy.graphicalFactory().createMaterial("treeBark", lrp);
   mat->addStage(stage);
   m_materialsStorage.add(mat);

   // prepare mesh
   TreesGenerator generator(treeParams, 8, 1, D3DXVECTOR3(1, 0, 0), 0.1f, 3, "treeBark");
   GraphicalEntityLoader loader(m_tamy.graphicalFactory(), m_materialsStorage);
   m_treeEntity = loader.load("tree", generator);
   
   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tree01");
   entInstance->attachEntity(*m_treeEntity);
   D3DXMATRIX rotMtx;
   D3DXMatrixRotationYawPitchRoll(&rotMtx, D3DXToRadian(180),0, 0);
   D3DXMatrixTranslation(&(entInstance->accessLocalMtx()), 0, -50, 100);
   D3DXMatrixMultiply(&(entInstance->accessLocalMtx()), &rotMtx, &(entInstance->accessLocalMtx()));

   // populate the scene
   demo::DynMeshesScene* dynamicScene = new demo::DynMeshesScene(new LinearStorage<RenderableNode>());
   dynamicScene->addNode(entInstance);

   m_animationController = m_treeEntity->instantiateSkeleton(*entInstance);
   m_animationController->activateAnimation("wind", true);
   timeController().get("animationTrack").add(new TTimeDependent<Skeleton>(*m_animationController));

   Light* light = m_tamy.graphicalFactory().createLight("light");
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

void TreesGeneratorDemo::deinitialize()
{   
   timeController().remove("rendererTrack");
   timeController().remove("animationTrack");

   delete m_camera;
   m_camera = NULL;

   delete m_renderingPipeline;
   m_renderingPipeline = NULL;

   delete m_animationController;
   m_animationController = NULL;

   delete m_treeEntity;
   m_treeEntity = NULL;
}


///////////////////////////////////////////////////////////////////////////////

DEMO(TreesGeneratorDemo)

///////////////////////////////////////////////////////////////////////////////