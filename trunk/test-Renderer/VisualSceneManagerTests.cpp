#include "core-TestFramework\TestFramework.h"
#include "VisualSceneManagerMock.h"
#include "core-Renderer\Camera.h"
#include "SkyBoxMock.h"
#include "core\MatrixWriter.h"
#include "TextureStub.h"
#include "MaterialOperationImplementationMock.h"
#include "LightReflectingPropertiesStub.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialOperation.h"
#include "GraphicalEntityMock.h"
#include "GraphicalNodeMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\NullCuller.h"


///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, addingCameraAndActiveCamera)
{
   VisualSceneManagerMock sceneManager;
   Camera* camera1 = new Camera("camera1");
   Camera* camera2 = new Camera("camera2");

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
   VisualSceneManagerMock sceneManager;
   Camera camera("camera");

   sceneManager.addNode(&camera);
   CPPUNIT_ASSERT_EQUAL(true, sceneManager.hasActiveCamera());
  
   sceneManager.removeNode(camera);
   CPPUNIT_ASSERT_EQUAL(false, sceneManager.hasActiveCamera());
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManager, changingActiveCameraReattachesSkyBox)
{
   VisualSceneManagerMock sceneManager;
   Camera* camera1 = new Camera("camera1");
   Camera* camera2 = new Camera("camera2");
   sceneManager.addNode(camera1);
   sceneManager.addNode(camera2);
   SkyBoxMock* skyBox = new SkyBoxMock();
   sceneManager.setSkyBox(skyBox);
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
   VisualSceneManagerMock sceneManager;
   Camera* camera = new Camera("camera");
   sceneManager.addNode(camera);

   SkyBoxMock* skyBox1 = new SkyBoxMock();
   SkyBoxMock* skyBox2 = new SkyBoxMock();

   D3DXMATRIX expectedResult;

   sceneManager.setSkyBox(skyBox1);
   D3DXMatrixIdentity(&expectedResult);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox1->getGlobalMtx());
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());

   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 20, 0, 0);
   D3DXMatrixTranslation(&expectedResult, 20, 0, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox1->getGlobalMtx());
   D3DXMatrixIdentity(&expectedResult);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());

   sceneManager.setSkyBox(skyBox2);
   D3DXMatrixTranslation(&expectedResult, 20, 0, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());

   D3DXMatrixTranslation(&(camera->accessLocalMtx()), 20, 40, 0);
   D3DXMatrixTranslation(&expectedResult, 20, 40, 0);
   CPPUNIT_ASSERT_EQUAL(expectedResult, skyBox2->getGlobalMtx());
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManagerTests, retrievingStaticGeometry)
{
   // prepare the materials
   TextureStub texture("");
   MaterialOperationImplementationMock matOpImpl;
   std::list<std::string> results;
   LightReflectingPropertiesStub lrp(results, 0);

   Material material1(lrp, matOpImpl, matOpImpl, 0);
   Material material2(lrp, matOpImpl, matOpImpl, 1);
   material1.addStage(new MaterialStage(texture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));
   material2.addStage(new MaterialStage(texture, 
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE)));

   // create the node we'll use for rendering
   std::vector<Material*> materials; 
   materials.push_back(&material1);
   materials.push_back(&material1);
   materials.push_back(&material2);
   materials.push_back(&material1);
   GraphicalEntityMock entity("entity", materials);

   GraphicalNode* node1 = new GraphicalNode("subset0 - material1", entity, 0);
   GraphicalNode* node2 = new GraphicalNode("subset1 - material1", entity, 1);
   GraphicalNode* node3 = new GraphicalNode("subset2 - material2", entity, 2);
   GraphicalNode* node4 = new GraphicalNode("subset3 - material1", entity, 3);

   Camera cameraNode("camera"); 

   // add the nodes to the scene
   VisualSceneManager sceneManager;
   sceneManager.setCuller(new NullCuller());
   sceneManager.setActiveCamera(cameraNode);

   // 1st node
   sceneManager.addNode(node1);
   DWORD arraySize = 0;
   AbstractGraphicalNodeP* nodes = sceneManager.getNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)1, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node1, nodes[0]);

   // 2nd node
   sceneManager.addNode(node2);
   nodes = sceneManager.getNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)2, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node2, nodes[1]);

   // 3rd node
   sceneManager.addNode(node3);
   nodes = sceneManager.getNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)3, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node3, nodes[2]);

   // 4th node - this one uses the same material as 1st and 2nd node, 
   //            so it should be grouped along with them (at least 
   //            with the batching strategy we used for this test)
   sceneManager.addNode(node4);
   nodes = sceneManager.getNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)4, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node4, nodes[2]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node3, nodes[3]);

   // remove one of the nodes

   sceneManager.removeNode(*node3); 
   delete node3;
   nodes = sceneManager.getNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)3, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node4, nodes[2]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManagerTests, transparentObjects)
{
   MaterialOperationImplementationMock matOpImpl;
   Camera cameraNode("camera");
   std::list<std::string> results;
   DWORD arraySize = 0;
   AbstractGraphicalNodeP* nodes;

   // prepare the materials
   TextureStub regularTexture(results);
   TextureStub transparentTexture(results);
   LightReflectingPropertiesStub lrp(results, 0);

   Material regularMaterial(lrp, matOpImpl, matOpImpl, 0);
   MaterialStage* regularMaterialStage = new MaterialStage(regularTexture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE));
   regularMaterial.addStage(regularMaterialStage);

   Material transparentMaterial(lrp, matOpImpl, matOpImpl, 1);
   MaterialStage* transparentMaterialStage = new MaterialStage(transparentTexture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_ADD, SC_NONE, SC_NONE));
   transparentMaterial.addStage(transparentMaterialStage);


   // create the node we'll use for rendering
   std::vector<Material*> materials; 
   materials.push_back(&regularMaterial);
   materials.push_back(&transparentMaterial);
   GraphicalEntityMock entity("entity", materials, results);

   GraphicalNode* regularNode     = new GraphicalNode("regularNode",     entity, 0);
   GraphicalNode* transparentNode = new GraphicalNode("transparentNode", entity, 1);


   // add the nodes to the scene
   VisualSceneManager sceneManager;
   sceneManager.setCuller(new NullCuller());
   sceneManager.setActiveCamera(cameraNode);
   sceneManager.addNode(regularNode);
   sceneManager.addNode(transparentNode);

   nodes = sceneManager.getNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)2, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)regularNode, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode, nodes[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManagerTests, removingTransparentObjects)
{
   MaterialOperationImplementationMock matOpImpl;
   Camera cameraNode("camera");
   std::list<std::string> results;
   DWORD arraySize = 0;
   AbstractGraphicalNodeP* nodes;

   // prepare the materials
   TextureStub transparentTexture(results);
   LightReflectingPropertiesStub lrp(results, 0);

   Material transparentMaterial(lrp, matOpImpl, matOpImpl, 0);
   MaterialStage* transparentMaterialStage = new MaterialStage(transparentTexture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_MULTIPLY, SC_NONE, SC_NONE));
   transparentMaterial.addStage(transparentMaterialStage);


   // create the node we'll use for rendering
   std::vector<Material*> materials;  materials.push_back(&transparentMaterial);
   GraphicalEntityMock entity("entity", materials, results);

   GraphicalNode* transparentNode1 = new GraphicalNode("transparentNode1", entity, 0);
   GraphicalNode* transparentNode2 = new GraphicalNode("transparentNode2", entity, 0);
   GraphicalNode* transparentNode3 = new GraphicalNode("transparentNode3", entity, 0);


   // add the nodes to the scene
   VisualSceneManager sceneManager;
   sceneManager.setCuller(new NullCuller());
   sceneManager.setActiveCamera(cameraNode);
   sceneManager.addNode(transparentNode1);
   sceneManager.addNode(transparentNode2);
   sceneManager.addNode(transparentNode3);

   nodes = sceneManager.getNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)3, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode3, nodes[2]);

   sceneManager.removeNode(*transparentNode2);
   delete transparentNode2;

   nodes = sceneManager.getNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)2, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode3, nodes[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisualSceneManagerTests, transparentObjectsAreSortedWithRespectToCamera)
{
   MaterialOperationImplementationMock matOpImpl;
   Camera cameraNode("camera");
   std::list<std::string> results;
   DWORD arraySize = 0;
   AbstractGraphicalNodeP* nodes;

   // prepare the materials
   TextureStub transparentTexture(results);
   LightReflectingPropertiesStub lrp(results, 0);

   Material transparentMaterial(lrp, matOpImpl, matOpImpl, 0);
   MaterialStage* transparentMaterialStage = new MaterialStage(transparentTexture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_SUBTRACT, SC_NONE, SC_NONE));
   transparentMaterial.addStage(transparentMaterialStage);


   // create the node we'll use for rendering
   std::vector<Material*> materials; materials.push_back(&transparentMaterial);
   GraphicalEntityMock entity("entity", materials, results);

   GraphicalNode* transparentNodeClose = new GraphicalNode("transparentNodeClose",  entity, 0);
   GraphicalNode* transparentNodeFar   = new GraphicalNode("transparentNodeFar",    entity, 0);

   // add the nodes to the scene
   VisualSceneManager sceneManager;
   sceneManager.setCuller(new NullCuller());
   sceneManager.setActiveCamera(cameraNode);
   sceneManager.addNode(transparentNodeClose);
   sceneManager.addNode(transparentNodeFar);

   D3DXMatrixTranslation(&(transparentNodeClose->accessLocalMtx()), 0, 0, 10);
   D3DXMatrixTranslation(&(transparentNodeFar->accessLocalMtx()),   0, 0, 50);

   // we're standing closer to the 'transparentNodeClose', so it's gonna be rendered last
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), 0, 0, 5);
   nodes = sceneManager.getNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)2, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNodeFar,   nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNodeClose, nodes[1]);


   // we moved next to the 'transparentNodeFar', so it's gonna be rendered last now
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), 0, 0, 55);
   nodes = sceneManager.getNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)2, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNodeClose, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNodeFar,   nodes[1]);
}

///////////////////////////////////////////////////////////////////////////////
