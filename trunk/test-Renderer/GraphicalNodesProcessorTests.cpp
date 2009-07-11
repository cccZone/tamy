#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\GraphicalNodesProcessor.h"
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
#include "core\Frustum.h"
#include "TransparencyEnablerStub.h"
#include "CoordinatesOperationMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalNodesProcessor, sortingByMaterial)
{
   // prepare the materials
   TextureStub texture("");
   MaterialOperationImplementationMock matOpImpl;
   std::list<std::string> results;
   TransparencyEnablerStub transparencyEnabler;

   Material material1("", new LightReflectingPropertiesStub(results, 0), matOpImpl, matOpImpl, transparencyEnabler);
   Material material2("", new LightReflectingPropertiesStub(results, 0), matOpImpl, matOpImpl, transparencyEnabler);
   material1.addStage(new MaterialStage(texture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new CoordinatesOperationMock(CC_WRAP)));
   material2.addStage(new MaterialStage(texture, 
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new CoordinatesOperationMock(CC_WRAP)));

   // create the node we'll use for rendering
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

   Camera cameraNode("camera"); 
   Array<AbstractGraphicalNode*> inputNodes;
   inputNodes.push_back(&node1);
   inputNodes.push_back(&node2);
   inputNodes.push_back(&node3);
   inputNodes.push_back(&node4);

   // run the test
   Array<AbstractGraphicalNode*> outputNodes;
   GraphicalNodesProcessor processor;

   processor(inputNodes, cameraNode.getFrustum(), D3DXVECTOR3(0, 0, 0), outputNodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, outputNodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&node1, outputNodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&node2, outputNodes[1]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&node4, outputNodes[2]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&node3, outputNodes[3]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalNodesProcessor, transparentObjects)
{
   MaterialOperationImplementationMock matOpImpl;
   Camera cameraNode("camera");
   std::list<std::string> results;

   // prepare the materials
   TextureStub regularTexture(results);
   TextureStub transparentTexture(results);
   TransparencyEnablerStub transparencyEnabler;

   Material regularMaterial("", new LightReflectingPropertiesStub(results, 0), matOpImpl, matOpImpl, transparencyEnabler);
   MaterialStage* regularMaterialStage = new MaterialStage(regularTexture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new CoordinatesOperationMock(CC_WRAP));
   regularMaterial.addStage(regularMaterialStage);

   Material transparentMaterial("", new LightReflectingPropertiesStub(results, 0), matOpImpl, matOpImpl, transparencyEnabler);
   MaterialStage* transparentMaterialStage = new MaterialStage(transparentTexture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_ADD, SC_NONE, SC_NONE),
      new CoordinatesOperationMock(CC_WRAP));
   transparentMaterial.addStage(transparentMaterialStage);


   // create the node we'll use for rendering
   std::vector<Material*> materials; 
   materials.push_back(&regularMaterial);
   materials.push_back(&transparentMaterial);
   GraphicalEntityMock entity("entity", materials, results);

   GraphicalNode regularNode("regularNode", false, entity, 0);
   GraphicalNode transparentNode("transparentNode", false, entity, 1);


   // add the nodes to the scene
   Array<AbstractGraphicalNode*> inputNodes;
   inputNodes.push_back(&regularNode);
   inputNodes.push_back(&transparentNode);

   // run the test
   Array<AbstractGraphicalNode*> outputNodes;
   GraphicalNodesProcessor processor;

   processor(inputNodes, cameraNode.getFrustum(), D3DXVECTOR3(0, 0, 0), outputNodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, outputNodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&regularNode, outputNodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&transparentNode, outputNodes[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalNodesProcessor, transparentObjectsAreSortedWithRespectToCamera)
{
   MaterialOperationImplementationMock matOpImpl;
   Camera cameraNode("camera");
   std::list<std::string> results;

   // prepare the materials
   TextureStub transparentTexture(results);
   TransparencyEnablerStub transparencyEnabler;

   Material transparentMaterial("", new LightReflectingPropertiesStub(results, 0), matOpImpl, matOpImpl, transparencyEnabler);
   MaterialStage* transparentMaterialStage = new MaterialStage(transparentTexture,
      new MaterialOperation(matOpImpl, MOP_DISABLE, SC_NONE, SC_NONE),
      new MaterialOperation(matOpImpl, MOP_SUBTRACT, SC_NONE, SC_NONE),
      new CoordinatesOperationMock(CC_WRAP));
   transparentMaterial.addStage(transparentMaterialStage);


   // create the node we'll use for rendering
   std::vector<Material*> materials; materials.push_back(&transparentMaterial);
   GraphicalEntityMock entity("entity", materials, results);

   GraphicalNode transparentNodeClose("transparentNodeClose", false, entity, 0);
   GraphicalNode transparentNodeFar("transparentNodeFar", false, entity, 0);


   // add the nodes to the scene
   Array<AbstractGraphicalNode*> inputNodes;
   inputNodes.push_back(&transparentNodeClose);
   inputNodes.push_back(&transparentNodeFar);

   // run the test
   Array<AbstractGraphicalNode*> outputNodes;
   GraphicalNodesProcessor processor;

   D3DXMatrixTranslation(&(transparentNodeClose.accessLocalMtx()), 0, 0, 10);
   D3DXMatrixTranslation(&(transparentNodeFar.accessLocalMtx()),   0, 0, 50);

   // we're standing closer to the 'transparentNodeClose', so it's gonna be rendered last
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), 0, 0, 5);
   outputNodes.clear();
   processor(inputNodes, cameraNode.getFrustum(), D3DXVECTOR3(0, 0, 5), outputNodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, outputNodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&transparentNodeFar,   outputNodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&transparentNodeClose, outputNodes[1]);


   // we moved next to the 'transparentNodeFar', so it's gonna be rendered last now
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), 0, 0, 55);
   outputNodes.clear();
   processor(inputNodes, cameraNode.getFrustum(), D3DXVECTOR3(0, 0, 55), outputNodes);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, outputNodes.size());
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&transparentNodeClose, outputNodes[0]);
   CPPUNIT_ASSERT_EQUAL((AbstractGraphicalNode*)&transparentNodeFar,   outputNodes[1]);
}

///////////////////////////////////////////////////////////////////////////////
