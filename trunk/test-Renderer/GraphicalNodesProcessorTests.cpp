#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\GraphicalNodesProcessor.h"
#include "core-Renderer\Camera.h"
#include "SkyBoxMock.h"
#include "core\MatrixWriter.h"
#include "GraphicalEntityMock.h"
#include "GraphicalNodeMock.h"
#include "core-Renderer\GraphicalNode.h"
#include "core\Frustum.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include "core-Renderer\ManagedTexture.h"
#include "RendererImplementationMock.h"


using namespace RegularTests;

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalNodesProcessor, sortingByMaterial)
{
   RendererImplementationMock renderer;

   // create the node we'll use for rendering
   Material material1("material1");
   Material material2("material1");
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

   Camera cameraNode("camera", renderer); 
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
   RendererImplementationMock renderer;
   Camera cameraNode("camera", renderer);
   std::vector<std::string> results;

   // create the node we'll use for rendering
   Material regularMaterial("regularMaterial");
   Material transparentMaterial("transparentMaterial");
   ManagedTexture tex("", new TTextureImpl<int>());
   transparentMaterial.addStage(new MaterialStage(tex, 
                                                  new MaterialOperation(MOP_SELECT_ARG1, SC_LRP, SC_NONE),
                                                  new MaterialOperation(MOP_SELECT_ARG1, SC_LRP, SC_NONE),
                                                  new CoordinatesOperation(CC_WRAP)));

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
   RendererImplementationMock renderer;
   Camera cameraNode("camera", renderer);
   std::vector<std::string> results;


   // create the node we'll use for rendering
   Material transparentMaterial("transparentMaterial");
   ManagedTexture tex("", new TTextureImpl<int>());
   transparentMaterial.addStage(new MaterialStage(tex, 
                                                  new MaterialOperation(MOP_SELECT_ARG1, SC_LRP, SC_NONE),
                                                  new MaterialOperation(MOP_SELECT_ARG1, SC_LRP, SC_NONE),
                                                  new CoordinatesOperation(CC_WRAP)));

   std::vector<Material*> materials;
   materials.push_back(&transparentMaterial);
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
