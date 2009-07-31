#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\Camera.h"
#include "SkyBoxMock.h"
#include "core\MatrixWriter.h"
#include "GraphicalEntityMock.h"
#include "GraphicalNodeMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\Material.h"
#include "core\Frustum.h"
#include "RendererImplementationMock.h"


using namespace RegularTests;

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, addingCameraAndActiveCamera)
{
   RendererImplementationMock renderer;
   VisualSceneManager sceneManager;
   Camera* camera1 = new Camera("camera1", renderer);
   Camera* camera2 = new Camera("camera2", renderer);

   CPPUNIT_ASSERT_EQUAL(false, sceneManager.hasActiveCamera());

   sceneManager.addNode(camera1);
   CPPUNIT_ASSERT_EQUAL(true, sceneManager.hasActiveCamera());
   CPPUNIT_ASSERT_EQUAL(std::string("camera1"), sceneManager.getActiveCamera().getName());

   sceneManager.addNode(camera2);
   CPPUNIT_ASSERT_EQUAL(true, sceneManager.hasActiveCamera());
   CPPUNIT_ASSERT_EQUAL(std::string("camera1"), sceneManager.getActiveCamera().getName());
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, removingCameraThatIsSetAsActive)
{
   RendererImplementationMock renderer;
   VisualSceneManager sceneManager;
   Camera camera("camera", renderer);

   sceneManager.addNode(&camera);
   CPPUNIT_ASSERT_EQUAL(true, sceneManager.hasActiveCamera());
  
   sceneManager.removeNode(camera);
   CPPUNIT_ASSERT_EQUAL(false, sceneManager.hasActiveCamera());
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, changingActiveCameraReattachesSkyBox)
{
   RendererImplementationMock renderer;
   VisualSceneManager sceneManager;
   Camera* camera1 = new Camera("camera1", renderer);
   Camera* camera2 = new Camera("camera2", renderer);
   sceneManager.addNode(camera1);
   sceneManager.addNode(camera2);
   SkyBoxMock* skyBox = new SkyBoxMock();
   sceneManager.addNode(skyBox);
   D3DXMatrixTranslation(&(camera2->accessLocalMtx()), 20, 0, 0);

   D3DXMATRIX expectedResult;

   sceneManager.setActiveCamera(*camera1);
   D3DXMatrixIdentity(&expectedResult);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox->getGlobalMtx());

   sceneManager.setActiveCamera(*camera2);
   D3DXMatrixIdentity(&expectedResult);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox->getLocalMtx());
   D3DXMatrixTranslation(&expectedResult, 20, 0, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox->getGlobalMtx());
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, addingSkyBoxAttachesItToActiveCamera)
{
   RendererImplementationMock renderer;
   VisualSceneManager sceneManager;
   Camera* camera = new Camera("camera", renderer);
   sceneManager.addNode(camera);

   SkyBoxMock* skyBox1 = new SkyBoxMock();
   SkyBoxMock* skyBox2 = new SkyBoxMock();

   D3DXMATRIX expectedResult;

   sceneManager.addNode(skyBox1);
   D3DXMatrixIdentity(&expectedResult);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox1->getGlobalMtx());
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());

   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 20, 0, 0);
   D3DXMatrixTranslation(&expectedResult, 20, 0, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox1->getGlobalMtx());
   D3DXMatrixIdentity(&expectedResult);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());

   sceneManager.addNode(skyBox2);
   D3DXMatrixTranslation(&expectedResult, 20, 0, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());

   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 20, 40, 0);
   D3DXMatrixTranslation(&expectedResult, 20, 40, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, retrievingStaticGeometry)
{
   RendererImplementationMock renderer;

   // create the node we'll use for rendering
   Material material1("");
   Material material2("");
   std::vector<Material*> materials; 
   materials.push_back(&material1);
   materials.push_back(&material1);
   materials.push_back(&material2);
   materials.push_back(&material1);
   GraphicalEntityMock entity("entity", materials);

   GraphicalNode node1("subset0 - material1", false, entity, 0);
   GraphicalNode node2("subset1 - material1", false, entity, 1);
   GraphicalNode node3("subset2 - material2", false, entity, 2);
   GraphicalNode node4("subset3 - material1", false, entity, 3);

   D3DXMatrixTranslation(&(node1.accessLocalMtx()), 0, 0, 50);
   D3DXMatrixTranslation(&(node2.accessLocalMtx()), 0, 0, 50);
   D3DXMatrixTranslation(&(node3.accessLocalMtx()), 0, 0, 50);
   D3DXMatrixTranslation(&(node4.accessLocalMtx()), 0, 0, 50);

   Camera cameraNode("camera", renderer); 

   // add the nodes to the scene
   VisualSceneManager sceneManager;
   sceneManager.setActiveCamera(cameraNode);

   Array<AbstractGraphicalNode*> nodes;

   // 1st node
   sceneManager.addNode(&node1);
   sceneManager.query<AbstractGraphicalNode>(cameraNode.getFrustum(), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node1, nodes[0]);

   // 2nd node
   nodes.clear();
   sceneManager.addNode(&node2);
   sceneManager.query<AbstractGraphicalNode>(cameraNode.getFrustum(), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node2, nodes[1]);

   // 3rd node
   nodes.clear();
   sceneManager.addNode(&node3);
   sceneManager.query<AbstractGraphicalNode>(cameraNode.getFrustum(), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node3, nodes[2]);

   // 4th node - this one uses the same material as 1st and 2nd node, 
   //            so it should be grouped along with them (at least 
   //            with the batching strategy we used for this test)
   nodes.clear();
   sceneManager.addNode(&node4);
   sceneManager.query<AbstractGraphicalNode>(cameraNode.getFrustum(), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node3, nodes[2]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node4, nodes[3]);

   // remove one of the nodes
   nodes.clear();
   sceneManager.removeNode(node3); 
   sceneManager.query<AbstractGraphicalNode>(cameraNode.getFrustum(), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node4, nodes[2]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, dynamicNodesAndOctrees)
{
   // create the node we'll use for rendering
   RendererImplementationMock renderer;
   std::vector<std::string> results;

   Material material("");
   std::vector<Material*> materials; 
   materials.push_back(&material);
   GraphicalEntityMock entity("entity", materials, results);
   GraphicalNode node1("node1", true, entity, 0);
   GraphicalNode node2("node2", false, entity, 0);
   GraphicalNode node3("node3", false, entity, 0);
   Camera cameraNode("camera", renderer); 

   // add the nodes to the scene
   VisualSceneManager sceneManager(1, 100);
   sceneManager.setActiveCamera(cameraNode);
   
   D3DXMatrixTranslation(&(node1.accessLocalMtx()), -50, 50, -10);
   D3DXMatrixTranslation(&(node2.accessLocalMtx()), -49, 50, -10);
   D3DXMatrixTranslation(&(node3.accessLocalMtx()), 50, 50, -10);
   sceneManager.addNode(&node1);
   sceneManager.addNode(&node2);
   sceneManager.addNode(&node3);

   Array<AbstractGraphicalNode*> nodes;

   // let's check the visibility of nodes in the positions they were in when
   // they were first added to the scene
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), -50, 50, -20);
   sceneManager.query<AbstractGraphicalNode>(cameraNode.getFrustum(), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node2, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node1, nodes[1]);

   // moving the camera - the dynamic node1 becomes invisible
   nodes.clear();
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), 50, 50, -20);
   sceneManager.query<AbstractGraphicalNode>(cameraNode.getFrustum(), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node3, nodes[0]);

   // now let's move node1 (a dynamic node) and see if the situation on the scene changes
   // accordingly
   nodes.clear();
   D3DXMatrixTranslation(&(node1.accessLocalMtx()), 49, 50, -10);
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), -50, 50, -20);
   sceneManager.query<AbstractGraphicalNode>(cameraNode.getFrustum(), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node2, nodes[0]);

   nodes.clear();
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), 50, 50, -20);
   sceneManager.query<AbstractGraphicalNode>(cameraNode.getFrustum(), nodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, nodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node3, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)&node1, nodes[1]);
}

///////////////////////////////////////////////////////////////////////////////
