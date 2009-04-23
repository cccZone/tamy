#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\BasicVisualSceneManager.h"
#include "core\Node.h"
#include "TextureStub.h"
#include "core-Renderer\Material.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "LightReflectingPropertiesStub.h"
#include "core-Renderer\BatchingStrategy.h"
#include "core-Renderer\Camera.h"


///////////////////////////////////////////////////////////////////////////////

TEST(BasicVisualSceneManagerTests, retrievingStaticGeometry)
{
   // prepare the materials
   TextureStub texture("");

   Material material1(texture, 0);
   Material material2(texture, 1);

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

   Camera cameraNode("camera"); // camera doesn't play any part as far as this scene manager is concerned

   // add the nodes to the scene
   BasicVisualSceneManager sceneManager;
   sceneManager.setActiveCamera(cameraNode);

   // 1st node
   sceneManager.addNode(node1);
   DWORD arraySize = 0;
   AbstractGraphicalNodeP* nodes = sceneManager.getRegularGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)1, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node1, nodes[0]);

   // 2nd node
   sceneManager.addNode(node2);
   nodes = sceneManager.getRegularGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)2, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node2, nodes[1]);

   // 3rd node
   sceneManager.addNode(node3);
   nodes = sceneManager.getRegularGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)3, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node3, nodes[2]);

   // 4th node - this one usesthe same material as 1st and 2nd node, 
   //            so it should be grouped toghether with them (at least 
   //            with the batching strategy we used for this test)
   sceneManager.addNode(node4);
   nodes = sceneManager.getRegularGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)4, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node4, nodes[2]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node3, nodes[3]);

   // remove one of the nodes

   sceneManager.removeNode(*node3); 
   delete node3;
   nodes = sceneManager.getRegularGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)3, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)node4, nodes[2]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(BasicVisualSceneManagerTests, transparentObjects)
{
   Camera cameraNode("camera");
   std::list<std::string> results;
   DWORD arraySize = 0;
   AbstractGraphicalNodeP* nodes;

   // prepare the materials
   TextureStub regularTexture(results, false);
   TextureStub transparentTexture(results, true);
   LightReflectingPropertiesStub lrp(results, 0);

   Material regularMaterial(regularTexture, 0);
   regularMaterial.setLightReflectingProperties(lrp);

   Material transparentMaterial(transparentTexture, 1);
   transparentMaterial.setLightReflectingProperties(lrp);


   // create the node we'll use for rendering
   std::vector<Material*> materials; 
   materials.push_back(&regularMaterial);
   materials.push_back(&transparentMaterial);
   GraphicalEntityMock entity("entity", materials, results);

   GraphicalNode* regularNode     = new GraphicalNode("regularNode",     entity, 0);
   GraphicalNode* transparentNode = new GraphicalNode("transparentNode", entity, 1);


   // add the nodes to the scene
   BasicVisualSceneManager sceneManager;
   sceneManager.setActiveCamera(cameraNode);
   sceneManager.addNode(regularNode);
   sceneManager.addNode(transparentNode);
   
   nodes = sceneManager.getRegularGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)1, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)regularNode, nodes[0]);

   nodes = sceneManager.getTransparentGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)1, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode, nodes[0]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(BasicVisualSceneManagerTests, removingTransparentObjects)
{
   Camera cameraNode("camera");
   std::list<std::string> results;
   DWORD arraySize = 0;
   AbstractGraphicalNodeP* nodes;

   // prepare the materials
   TextureStub transparentTexture(results, true);
   LightReflectingPropertiesStub lrp(results, 0);
   Material transparentMaterial(transparentTexture, 0);
   transparentMaterial.setLightReflectingProperties(lrp);


   // create the node we'll use for rendering
   std::vector<Material*> materials;  materials.push_back(&transparentMaterial);
   GraphicalEntityMock entity("entity", materials, results);

   GraphicalNode* transparentNode1 = new GraphicalNode("transparentNode1", entity, 0);
   GraphicalNode* transparentNode2 = new GraphicalNode("transparentNode2", entity, 0);
   GraphicalNode* transparentNode3 = new GraphicalNode("transparentNode3", entity, 0);


   // add the nodes to the scene
   BasicVisualSceneManager sceneManager;
   sceneManager.setActiveCamera(cameraNode);
   sceneManager.addNode(transparentNode1);
   sceneManager.addNode(transparentNode2);
   sceneManager.addNode(transparentNode3);

   nodes = sceneManager.getTransparentGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)3, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode2, nodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode3, nodes[2]);

   sceneManager.removeNode(*transparentNode2);
   delete transparentNode2;

   nodes = sceneManager.getTransparentGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)2, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode1, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNode3, nodes[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(BasicVisualSceneManagerTests, transparentObjectsAreSortedWithRespectToCamera)
{
   Camera cameraNode("camera");
   std::list<std::string> results;
   DWORD arraySize = 0;
   AbstractGraphicalNodeP* nodes;

   // prepare the materials
   TextureStub transparentTexture(results, true);
   LightReflectingPropertiesStub lrp(results, 0);
   Material transparentMaterial(transparentTexture, 0);
   transparentMaterial.setLightReflectingProperties(lrp);


   // create the node we'll use for rendering
   std::vector<Material*> materials; materials.push_back(&transparentMaterial);
   GraphicalEntityMock entity("entity", materials, results);

   GraphicalNode* transparentNodeClose = new GraphicalNode("transparentNodeClose",  entity, 0);
   GraphicalNode* transparentNodeFar   = new GraphicalNode("transparentNodeFar",    entity, 0);

   // add the nodes to the scene
   BasicVisualSceneManager sceneManager;
   sceneManager.setActiveCamera(cameraNode);
   sceneManager.addNode(transparentNodeClose);
   sceneManager.addNode(transparentNodeFar);

   D3DXMatrixTranslation(&(transparentNodeClose->accessLocalMtx()), 0, 0, 10);
   D3DXMatrixTranslation(&(transparentNodeFar->accessLocalMtx()),   0, 0, 50);

   // we're standing closer to the 'transparentNodeClose', so it's gonna be rendered last
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), 0, 0, 5);
   nodes = sceneManager.getTransparentGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)2, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNodeFar,   nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNodeClose, nodes[1]);


   // we moved next to the 'transparentNodeFar', so it's gonna be rendered last now
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), 0, 0, 55);
   nodes = sceneManager.getTransparentGraphicalNodes(arraySize);
   CPPUNIT_ASSERT_EQUAL((DWORD)2, arraySize);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNodeClose, nodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNodeP)transparentNodeFar,   nodes[1]);
}

///////////////////////////////////////////////////////////////////////////////
