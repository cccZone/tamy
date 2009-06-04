#include "TreesGeneratorDemo.h"
#include "impl-DirectX\D3DApplicationManager.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-Renderer\Renderer.h"
#include "core\Point.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "core\CompositeSceneManager.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\Skeleton.h"
#include "ext-TreesGenerator\TreeParams.h"
#include "ext-TreesGenerator\TreeStructureGenerator.h"
#include "ext-TreesGenerator\TreeSkinner.h"
#include "ext-TreesGenerator\TreeSegment.h"
#include "ext-TreesGenerator\TreeAnimator.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"


///////////////////////////////////////////////////////////////////////////////

TreesGeneratorDemo::TreesGeneratorDemo()
      : Application("Demo"),
      m_renderer(NULL),
      m_resourceManager(NULL),
      m_sceneManager(NULL),
      m_cameraController(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

void TreesGeneratorDemo::initialize(Renderer& renderer, ResourceManager& resourceManager)
{
   m_renderer = &renderer;
   m_resourceManager = &resourceManager;

   m_rotating = false;
   m_sceneManager = new CompositeSceneManager();
   VisualSceneManager* visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(visualSceneManager);
   m_renderer->addVisualSceneManager(*visualSceneManager);

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

   TreeStructureGenerator generator;
   TreeSegment* treeStruct = generator.generate(treeParams);

   TreeSkinner skinner(*treeStruct);
   MaterialDefinition treeBark("treeBark");
   treeBark.ambient.r = 1;
   treeBark.ambient.g = 1;
   treeBark.ambient.b = 1;
   treeBark.ambient.a = 1;
   treeBark.diffuse.r = 1;
   treeBark.diffuse.g = 1;
   treeBark.diffuse.b = 1;
   treeBark.diffuse.a = 1;
   treeBark.matName = "treeBark";
   treeBark.texName = "LondonPlaneBark.dds";

   MeshDefinition* mesh = skinner("tree", 8, 1, treeBark);
   delete treeStruct;

   TreeAnimator animator;
   AnimationDefinition anim;
   animator(*mesh, D3DXVECTOR3(1, 0, 0), 0.1f, 3, anim);

   AbstractGraphicalEntity* treeEntity = m_resourceManager->createGraphicalEntityFromTemplate(*mesh);
   m_resourceManager->registerGraphicalEntity("tree", treeEntity);
   treeEntity->setAnimationDefinition(anim);

   delete mesh;

   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tree01", false);
   entInstance->attachEntity(*treeEntity);
   m_sceneManager->addNode(entInstance);

   m_animationController = treeEntity->instantiateSkeleton(*entInstance);
   m_animationController->activateAnimation("wind", true);

   Light* light = m_resourceManager->createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 0));
   light->setLookVec(D3DXVECTOR3(0, 0, -1));
   m_sceneManager->addNode(light);

   Camera* camera = m_resourceManager->createCamera("camera");
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
   delete m_animationController;
   m_animationController = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;

   m_renderer = NULL;
   m_resourceManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TreesGeneratorDemo::update(float timeElapsed)
{
   float movementSpeed = 40 * timeElapsed;
   float rotationSpeed = 180 * timeElapsed;


   // process the keys
   if (context().isKeyPressed(VK_ESCAPE))     {context().signal(*this, ON_EXIT);}
   if (context().isKeyPressed(VK_UP))     {m_cameraController->move(m_cameraController->getLookVec()   * movementSpeed);}
   if (context().isKeyPressed(VK_DOWN))   {m_cameraController->move(-m_cameraController->getLookVec()  * movementSpeed);}
   if (context().isKeyPressed(VK_LEFT))   {m_cameraController->move(-m_cameraController->getRightVec() * movementSpeed);}
   if (context().isKeyPressed(VK_RIGHT))  {m_cameraController->move(m_cameraController->getRightVec()  * movementSpeed);}

   if (context().isKeyPressed(VK_LBUTTON) && (m_rotating == false))
   {
      context().relativeMouseMovement(true);
      m_rotating = true;
   }
   else if ((context().isKeyPressed(VK_LBUTTON) == false) && m_rotating)
   {
      context().relativeMouseMovement(false);
      m_rotating = false;
   }

   // process the mouse
   if (m_rotating)
   {
      Point mouseRel = context().getMousePos();
      float rotX = (float)(mouseRel.x) / 3.0f;
      float rotY = (float)(mouseRel.y) / 3.0f;
      m_cameraController->rotate(rotY * rotationSpeed, rotX * rotationSpeed, 0);
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
   D3DApplicationManager applicationManager(hInstance, nCmdShow, "Trees Generator Demo");
	TreesGeneratorDemo app;

   applicationManager.addApplication(app);
   applicationManager.setEntryApplication(app.getName());

   while (applicationManager.step()) {}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
