#include "TestFramework.h"
#include "GraphicalNode.h"
#include "RenderingProcessor.h"
#include "RenderingCommand.h"
#include "Material.h"
#include "LightReflectingPropertiesStub.h"
#include "TextureStub.h"
#include "MatrixWriter.h"
#include "GraphicalEntityMock.h"
#include "GraphicalNodesAggregator.h"
#include <vector>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEntity, referencesToOneEntity)
{
   std::vector<Material*> materialsListStub;
   materialsListStub.push_back(reinterpret_cast<Material*> (0xCC));

   GraphicalEntityMock entity(materialsListStub);

   Node* reference1 = entity.instantiate("reference1");
   Node* reference2 = entity.instantiate("reference2");

   D3DXMATRIX mtx1;
   D3DXMatrixTranslation(&mtx1, 10, 0, 0);
   reference1->setLocalMtx(mtx1);

   D3DXMATRIX mtx2;
   D3DXMatrixTranslation(&mtx2, -10, 0, 0);
   reference2->setLocalMtx(mtx2);

   dynamic_cast<GraphicalNode*> (reference1)->render();
   CPPUNIT_ASSERT_EQUAL(mtx1, entity.getMatrixSet());

   dynamic_cast<GraphicalNode*> (reference2)->render();
   CPPUNIT_ASSERT_EQUAL(mtx2, entity.getMatrixSet());

   delete reference1;
   delete reference2;
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEntity, creatingNodeForAntityWithManySubsets)
{
   TextureStub texture("");
   Material mat1(texture);
   Material mat2(texture);
   Material mat3(texture);

   std::vector<Material*> materials;
   materials.push_back(&mat1);
   materials.push_back(&mat2);
   materials.push_back(&mat3);

   // prepare the entities
   GraphicalEntityMock richBody(materials);
   Node* node = richBody.instantiate("node");

   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (2), node->getChildrenCount());
   
   std::vector<std::string> expectedChildren;
   expectedChildren.push_back("node");
   expectedChildren.push_back("node_subset1");
   expectedChildren.push_back("node_subset2");

   GraphicalNodesAggregator<GraphicalNode> children;
   node->accept(children);

   unsigned int expChildIdx = 0;
   for (std::list<GraphicalNode*>::const_iterator it = children().begin();
        it != children().end(); ++it, ++expChildIdx)
   {
      CPPUNIT_ASSERT_EQUAL(expectedChildren.at(expChildIdx), (*it)->getName());
   }

   delete node;
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEntity, renderingEntityWithManySubsets)
{
   std::list<std::string> results;

   TextureStub texture("");
   Material mat1(texture);
   Material mat2(texture);
   Material mat3(texture);

   std::vector<Material*> materials;
   materials.push_back(&mat1);
   materials.push_back(&mat2);
   materials.push_back(&mat3);

   // prepare the entities
   GraphicalEntityMock richBody("richBody", materials, results);
   Node* node = richBody.instantiate("node");

   GraphicalNodesAggregator<GraphicalNode> nodesToRender;
   node->accept(nodesToRender);

   RenderingProcessor processor;
   std::list<RenderingCommand> commands = processor.translate(nodesToRender());
   
   // run through the comands
   for (std::list<RenderingCommand>::iterator it = commands.begin(); it != commands.end(); ++it)
   {
      RenderingCommand& comm = *it;
      comm();
   }

   // compare the results
   std::vector<std::string> expectedResults;
   expectedResults.push_back("Render richBody subset 0");
   expectedResults.push_back("Render richBody subset 1");
   expectedResults.push_back("Render richBody subset 2");

   CPPUNIT_ASSERT_EQUAL(expectedResults.size(), results.size());

   unsigned int resultIdx = 0;
   for (std::list<std::string>::iterator resultIt = results.begin(); 
        resultIt != results.end(); ++resultIt, ++resultIdx)
   {
      std::stringstream msgId;
      msgId << "command " << resultIdx;
      CPPUNIT_ASSERT_EQUAL_MESSAGE(msgId.str(), expectedResults.at(resultIdx), *resultIt);
   }

   delete node;
}

///////////////////////////////////////////////////////////////////////////////

TEST(HierarchicalEntity, basicCreation)
{
   std::list<std::string> results;
   std::vector<Material*> materialsListStub;
   materialsListStub.push_back(reinterpret_cast<Material*> (0xCC));

   // prepare the entities
   GraphicalEntityMock body("body", materialsListStub, results);
   GraphicalEntityMock head("head", materialsListStub, results);
   GraphicalEntityMock hand("hand", materialsListStub, results);
   body.addChild(head);
   body.addChild(hand);

   Node* node = body.instantiate("node");

   CPPUNIT_ASSERT_EQUAL(std::string("body"), dynamic_cast<GraphicalNode*> (node)->getEntity().getName());
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (2), node->getChildrenCount());
   
   std::vector<std::string> expectedChildren;
   expectedChildren.push_back("body");
   expectedChildren.push_back("head");
   expectedChildren.push_back("hand");

   GraphicalNodesAggregator<GraphicalNode> children;
   node->accept(children);
   unsigned int expChildIdx = 0;
   for (std::list<GraphicalNode*>::const_iterator it = children().begin();
        it != children().end(); ++it, ++expChildIdx)
   {
      CPPUNIT_ASSERT_EQUAL(expectedChildren.at(expChildIdx), (*it)->getEntity().getName());
   }

   delete node;
}

///////////////////////////////////////////////////////////////////////////////

TEST(HierarchicalEntity, matricesOfGraphicalNodesSet)
{
   std::vector<Material*> materialsListStub;
   materialsListStub.push_back(reinterpret_cast<Material*> (0xCC));

   D3DXMATRIX bodyMtx; D3DXMatrixIdentity(&bodyMtx);
   D3DXMATRIX headMtx; D3DXMatrixTranslation(&headMtx, 0, -5, 0);
   D3DXMATRIX handMtx; D3DXMatrixTranslation(&handMtx, 5, 0, 0);

   // prepare the entities
   GraphicalEntityMock body(materialsListStub, bodyMtx);
   GraphicalEntityMock head(materialsListStub, headMtx);
   GraphicalEntityMock hand(materialsListStub, handMtx);

   body.addChild(head);
   body.addChild(hand);

   Node* node = body.instantiate("node");


   std::vector<D3DXMATRIX> expectedMatrices;
   expectedMatrices.push_back(bodyMtx);
   expectedMatrices.push_back(headMtx);
   expectedMatrices.push_back(handMtx);

   GraphicalNodesAggregator<GraphicalNode> children;
   node->accept(children);
   unsigned int expChildIdx = 0;
   for (std::list<GraphicalNode*>::const_iterator it = children().begin();
        it != children().end(); ++it, ++expChildIdx)
   {
      CPPUNIT_ASSERT_EQUAL(expectedMatrices.at(expChildIdx), (*it)->getLocalMtx());
   }

   delete node;
}

///////////////////////////////////////////////////////////////////////////////

TEST(HierarchicalEntity, creatingNodesAroundSpacerEntities)
{
   std::list<std::string> results;
   std::vector<Material*> materialsListStub;
   materialsListStub.push_back(reinterpret_cast<Material*> (0xCC));
   std::vector<Material*> emptyMaterialsListStub;

   // prepare the entities
   GraphicalEntityMock raymanBody("raymanBody", emptyMaterialsListStub, results);
   GraphicalEntityMock head("head", materialsListStub, results);
   GraphicalEntityMock hand("hand", materialsListStub, results);

   raymanBody.addChild(head);
   raymanBody.addChild(hand);

   Node* node = raymanBody.instantiate("node");

   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int> (2), node->getChildrenCount());
   
   std::vector<std::string> expectedChildren;
   // no rayman body - it's not gonna be a graphical node
   expectedChildren.push_back("head");
   expectedChildren.push_back("hand");

   GraphicalNodesAggregator<GraphicalNode> children;
   node->accept(children);
   unsigned int expChildIdx = 0;
   for (std::list<GraphicalNode*>::const_iterator it = children().begin();
        it != children().end(); ++it, ++expChildIdx)
   {
      CPPUNIT_ASSERT_EQUAL(expectedChildren.at(expChildIdx), (*it)->getEntity().getName());
   }
}

///////////////////////////////////////////////////////////////////////////////
