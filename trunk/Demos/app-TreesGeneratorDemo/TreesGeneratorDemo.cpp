#include "TreesGeneratorDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Skeleton.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-Demo\DemoRendererDefinition.h"
#include "ext-Demo\LightsScene.h"
#include "ext-TreesGenerator\TreeParams.h"
#include "ext-TreesGenerator\TreesGenerator.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

TreesGeneratorDemo::TreesGeneratorDemo(Tamy& tamy)
: DemoApp(tamy)
, m_treeEntity(NULL)
{
   timeController().add("animationTrack");
}

///////////////////////////////////////////////////////////////////////////////

void TreesGeneratorDemo::initializeScene(demo::DynMeshesScene& dynamicScene, 
                                         demo::LightsScene& lights)
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
   dynamicScene.addNode(entInstance);

   m_animationController = m_treeEntity->instantiateSkeleton(*entInstance);
   m_animationController->activateAnimation("wind", true);
   timeController().get("animationTrack").add(new TTimeDependent<Skeleton>(*m_animationController));

   Light* light = m_tamy.graphicalFactory().createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 0));
   light->setLookVec(D3DXVECTOR3(0, 0, 1));
   lights.insert(light);
}

///////////////////////////////////////////////////////////////////////////////

void TreesGeneratorDemo::onDeinitialize()
{   
   timeController().remove("animationTrack");

   delete m_animationController;
   m_animationController = NULL;

   delete m_treeEntity;
   m_treeEntity = NULL;
}


///////////////////////////////////////////////////////////////////////////////

DEMO(TreesGeneratorDemo)

///////////////////////////////////////////////////////////////////////////////
