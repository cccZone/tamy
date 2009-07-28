#include "TreesGeneratorDemo.h"
#include "tamy\Tamy.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-AppFlow\UserInputController.h"
#include "core-AppFlow\ApplicationManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core\Point.h"
#include "core\CompositeSceneManager.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\Skeleton.h"
#include "ext-TreesGenerator\TreeParams.h"
#include "ext-TreesGenerator\TreesGenerator.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\SceneRenderingMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"


///////////////////////////////////////////////////////////////////////////////

TreesGeneratorDemo::TreesGeneratorDemo(Tamy& tamy)
      : Application("Demo"),
      m_renderer(&(tamy.renderer())),
      m_tamy(tamy),
      m_renderingTarget(NULL),
      m_uiController(m_tamy.uiController()),
      m_treeEntity(NULL),
      m_sceneManager(NULL),
      m_cameraController(NULL)
{
   timeController().add("regularTrack");
   timeController().get("regularTrack").add(new TTimeDependent<TreesGeneratorDemo>(*this));
}

///////////////////////////////////////////////////////////////////////////////

void TreesGeneratorDemo::initialize()
{
   m_rotating = false;

   SettableRenderingTargetsPolicy* sceneRenderingTargetPolicy = new SettableRenderingTargetsPolicy();
   SceneRenderingMechanism* sceneRenderer = m_tamy.createSceneRenderingMechanism(sceneRenderingTargetPolicy);
   m_renderer->addMechanism(sceneRenderer);
   m_renderingTarget = m_tamy.graphicalFactory().createDefaultRenderingTarget();
   sceneRenderingTargetPolicy->addTarget(0, *m_renderingTarget);

   m_sceneManager = new CompositeSceneManager();
   VisualSceneManager* visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(visualSceneManager);
   sceneRenderer->addVisualSceneManager(*visualSceneManager);

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
   m_renderingTechniquesStorage.add(mat);

   // prepare mesh
   TreesGenerator generator(treeParams, 8, 1, D3DXVECTOR3(1, 0, 0), 0.1f, 3, "treeBark");
   GraphicalEntityLoader loader(m_tamy.graphicalFactory(), m_renderingTechniquesStorage);
   m_treeEntity = loader.load("tree", generator);
   
   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tree01", false);
   entInstance->attachEntity(*m_treeEntity);
   m_sceneManager->addNode(entInstance);

   m_animationController = m_treeEntity->instantiateSkeleton(*entInstance);
   m_animationController->activateAnimation("wind", true);

   Light* light = m_tamy.graphicalFactory().createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 0));
   light->setLookVec(D3DXVECTOR3(0, 0, -1));
   m_sceneManager->addNode(light);

   Camera* camera = m_tamy.graphicalFactory().createCamera("camera");
   D3DXMATRIX rotMtx;
   D3DXMatrixRotationYawPitchRoll(&rotMtx, D3DXToRadian(180), 0, 0);
   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 0, 50, 100);
   D3DXMatrixMultiply(&(camera->accessLocalMtx()), &rotMtx, &(camera->accessLocalMtx()));
   m_sceneManager->addNode(camera);
   m_cameraController = new UnconstrainedMotionController(*camera);
}

///////////////////////////////////////////////////////////////////////////////

void TreesGeneratorDemo::deinitialize()
{
   delete m_renderingTarget;
   m_renderingTarget = NULL;

   delete m_animationController;
   m_animationController = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;

   delete m_treeEntity;
   m_treeEntity = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TreesGeneratorDemo::update(float timeElapsed)
{
   m_uiController.update(timeElapsed);

   float movementSpeed = 40 * timeElapsed;
   float rotationSpeed = 0.1f * timeElapsed;


   // process the keys
   if (m_uiController.isKeyPressed(VK_ESCAPE))     {context().signal(*this, ON_EXIT);}
   if (m_uiController.isKeyPressed(VK_UP))     {m_cameraController->move(m_cameraController->getLookVec()   * movementSpeed);}
   if (m_uiController.isKeyPressed(VK_DOWN))   {m_cameraController->move(-m_cameraController->getLookVec()  * movementSpeed);}
   if (m_uiController.isKeyPressed(VK_LEFT))   {m_cameraController->move(-m_cameraController->getRightVec() * movementSpeed);}
   if (m_uiController.isKeyPressed(VK_RIGHT))  {m_cameraController->move(m_cameraController->getRightVec()  * movementSpeed);}

   if (m_uiController.isKeyPressed(VK_LBUTTON) && (m_rotating == false))
   {
      m_uiController.setRelativeMouseMovement(true);
      m_rotating = true;
   }
   else if ((m_uiController.isKeyPressed(VK_LBUTTON) == false) && m_rotating)
   {
      m_uiController.setRelativeMouseMovement(false);
      m_rotating = false;
   }

   // process the mouse
   if (m_rotating)
   {
      D3DXVECTOR2 mouseSpeed = m_uiController.getMouseSpeed() * rotationSpeed;
      m_cameraController->rotate(mouseSpeed.y, mouseSpeed.x, 0);
   }

   m_animationController->update(timeElapsed);
   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   Tamy::initialize(hInstance, nCmdShow, "Trees Generator Demo", 1024, 768, false);

   // create the application components
	TreesGeneratorDemo app(TAMY);

   ApplicationManager& appMgr = TAMY.appManager();

   appMgr.addApplication(app);
   appMgr.setEntryApplication(app.getName());

   // run the app
   while (appMgr.step()) {Sleep(0);}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
