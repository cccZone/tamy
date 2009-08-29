#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\GraphicalNodesProcessor.h"
#include "core-Renderer\Camera.h"
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

   GraphicalNode node1("subset0 - material1", entity, 0);
   GraphicalNode node2("subset1 - material1", entity, 1);
   GraphicalNode node3("subset2 - material2", entity, 2);
   GraphicalNode node4("subset3 - material1", entity, 3);

   Camera cameraNode("camera", renderer); 
   Array<RenderableNode*> inputNodes;
   inputNodes.push_back(&node1);
   inputNodes.push_back(&node2);
   inputNodes.push_back(&node3);
   inputNodes.push_back(&node4);

   // run the test
   Array<Renderable*> output;
   GraphicalNodesProcessor processor;

   processor(inputNodes, cameraNode.getFrustum(), D3DXVECTOR3(0, 0, 0), output);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, output.size());
   CPPUNIT_ASSERT_EQUAL(dynamic_cast<Renderable*> (&node1), output[0]);
   CPPUNIT_ASSERT_EQUAL(dynamic_cast<Renderable*> (&node2), output[1]);
   CPPUNIT_ASSERT_EQUAL(dynamic_cast<Renderable*> (&node4), output[2]);
   CPPUNIT_ASSERT_EQUAL(dynamic_cast<Renderable*> (&node3), output[3]);
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

   GraphicalNode regularNode("regularNode", entity, 0);
   GraphicalNode transparentNode("transparentNode", entity, 1);


   // add the nodes to the scene
   Array<RenderableNode*> inputNodes;
   inputNodes.push_back(&regularNode);
   inputNodes.push_back(&transparentNode);

   // run the test
   Array<Renderable*> output;
   GraphicalNodesProcessor processor;

   processor(inputNodes, cameraNode.getFrustum(), D3DXVECTOR3(0, 0, 0), output);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, output.size());
   CPPUNIT_ASSERT_EQUAL(dynamic_cast<Renderable*> (&regularNode), output[0]);
   CPPUNIT_ASSERT_EQUAL(dynamic_cast<Renderable*> (&transparentNode), output[1]);
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

   GraphicalNode transparentNodeClose("transparentNodeClose", entity, 0);
   GraphicalNode transparentNodeFar("transparentNodeFar", entity, 0);


   // add the nodes to the scene
   Array<RenderableNode*> inputNodes;
   inputNodes.push_back(&transparentNodeClose);
   inputNodes.push_back(&transparentNodeFar);

   // run the test
   Array<Renderable*> output;
   GraphicalNodesProcessor processor;

   D3DXMatrixTranslation(&(transparentNodeClose.accessLocalMtx()), 0, 0, 10);
   D3DXMatrixTranslation(&(transparentNodeFar.accessLocalMtx()),   0, 0, 50);

   // we're standing closer to the 'transparentNodeClose', so it's gonna be rendered last
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), 0, 0, 5);
   output.clear();
   processor(inputNodes, cameraNode.getFrustum(), D3DXVECTOR3(0, 0, 5), output);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, output.size());
   CPPUNIT_ASSERT_EQUAL(dynamic_cast<Renderable*> (&transparentNodeFar),   output[0]);
   CPPUNIT_ASSERT_EQUAL(dynamic_cast<Renderable*> (&transparentNodeClose), output[1]);


   // we moved next to the 'transparentNodeFar', so it's gonna be rendered last now
   D3DXMatrixTranslation(&(cameraNode.accessLocalMtx()), 0, 0, 55);
   output.clear();
   processor(inputNodes, cameraNode.getFrustum(), D3DXVECTOR3(0, 0, 55), output);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, output.size());
   CPPUNIT_ASSERT_EQUAL(dynamic_cast<Renderable*> (&transparentNodeClose), output[0]);
   CPPUNIT_ASSERT_EQUAL(dynamic_cast<Renderable*> (&transparentNodeFar),   output[1]);
}

///////////////////////////////////////////////////////////////////////////////
