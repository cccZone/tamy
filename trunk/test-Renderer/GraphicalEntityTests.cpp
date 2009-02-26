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
#include "NodeHierarchyWriter.h"


///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEntityInstantiation, basics)
{
   std::vector<Material*> materialsListStub;
   materialsListStub.push_back(reinterpret_cast<Material*> (0xCC));

   D3DXMATRIX bodyMtx; D3DXMatrixIdentity(&bodyMtx);
   D3DXMATRIX headMtx; D3DXMatrixTranslation(&headMtx, 0, -5, 0);
   D3DXMATRIX handMtx; D3DXMatrixTranslation(&handMtx, 5, 0, 0);

   // prepare the entities
   GraphicalEntityMock body("body", materialsListStub, bodyMtx);
   GraphicalEntityMock head("head", materialsListStub, headMtx);
   GraphicalEntityMock hand("hand", materialsListStub, handMtx);

   body.addChild(head);
   body.addChild(hand);

   Node* node = body.instantiate("node");

   // prepare the results
   Node* expectedHierarchy = new Node("node");

   Node* bodyNode = new Node("body");
   bodyNode->setLocalMtx(bodyMtx);
   bodyNode->addChild(new GraphicalNode("body_subset0", body, 0));

   Node* headNode = new Node("head");
   headNode->setLocalMtx(headMtx);
   headNode->addChild(new GraphicalNode("head_subset0", head, 0));

   Node* handNode = new Node("hand");
   handNode->setLocalMtx(handMtx);
   handNode->addChild(new GraphicalNode("hand_subset0", hand, 0));

   bodyNode->addChild(headNode);
   bodyNode->addChild(handNode);
   expectedHierarchy->addChild(bodyNode);

   NodeHierarchyWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedHierarchy), writer(*node));

   delete node;
   delete expectedHierarchy;
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEntityInstantiation, multipleSubsets)
{
   TextureStub texture("");
   Material skinMat(texture);
   Material blouseMat(texture);
   Material hairMat(texture);

   std::vector<Material*> bodyMaterials;
   std::vector<Material*> headMaterials;
   std::vector<Material*> handMaterials;

   bodyMaterials.push_back(&skinMat);
   bodyMaterials.push_back(&blouseMat);
   headMaterials.push_back(&skinMat);
   headMaterials.push_back(&hairMat);
   handMaterials.push_back(&skinMat);

   D3DXMATRIX bodyMtx; D3DXMatrixIdentity(&bodyMtx);
   D3DXMATRIX headMtx; D3DXMatrixTranslation(&headMtx, 0, -5, 0);
   D3DXMATRIX handMtx; D3DXMatrixTranslation(&handMtx, 5, 0, 0);

   // prepare the entities
   GraphicalEntityMock body("body", bodyMaterials, bodyMtx);
   GraphicalEntityMock head("head", headMaterials, headMtx);
   GraphicalEntityMock hand("hand", handMaterials, handMtx);

   body.addChild(head);
   body.addChild(hand);

   Node* node = body.instantiate("node");

   // prepare the results
   Node* expectedHierarchy = new Node("node");

   Node* bodyNode = new Node("body");
   bodyNode->setLocalMtx(bodyMtx);
   bodyNode->addChild(new GraphicalNode("body_subset0", body, 0));
   bodyNode->addChild(new GraphicalNode("body_subset1", body, 1));

   Node* headNode = new Node("head");
   headNode->setLocalMtx(headMtx);
   headNode->addChild(new GraphicalNode("head_subset0", head, 0));
   headNode->addChild(new GraphicalNode("head_subset1", head, 1));

   Node* handNode = new Node("hand");
   handNode->setLocalMtx(handMtx);
   handNode->addChild(new GraphicalNode("hand_subset0", hand, 0));

   bodyNode->addChild(headNode);
   bodyNode->addChild(handNode);
   expectedHierarchy->addChild(bodyNode);

   NodeHierarchyWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedHierarchy), writer(*node));

   delete node;
   delete expectedHierarchy;
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEntityInstantiation, creatingNodesAroundSpacerEntities)
{
   std::vector<Material*> materialsListStub;
   materialsListStub.push_back(reinterpret_cast<Material*> (0xCC));
   std::vector<Material*> emptyMaterialsListStub;

   D3DXMATRIX bodyMtx; D3DXMatrixIdentity(&bodyMtx);
   D3DXMATRIX headMtx; D3DXMatrixTranslation(&headMtx, 0, -5, 0);
   D3DXMATRIX handMtx; D3DXMatrixTranslation(&handMtx, 5, 0, 0);

   // prepare the entities
   GraphicalEntityMock raymanBody("raymanBody", emptyMaterialsListStub, bodyMtx);
   GraphicalEntityMock head("head", materialsListStub, headMtx);
   GraphicalEntityMock hand("hand", materialsListStub, handMtx);

   raymanBody.addChild(head);
   raymanBody.addChild(hand);

   Node* node = raymanBody.instantiate("node");

   // prepare the results
   Node* expectedHierarchy = new Node("node");

   Node* raymanBodyNode = new Node("raymanBody");
   raymanBodyNode->setLocalMtx(bodyMtx);

   Node* headNode = new Node("head");
   headNode->setLocalMtx(headMtx);
   headNode->addChild(new GraphicalNode("head_subset0", head, 0));

   Node* handNode = new Node("hand");
   handNode->setLocalMtx(handMtx);
   handNode->addChild(new GraphicalNode("hand_subset0", hand, 0));

   raymanBodyNode->addChild(headNode);
   raymanBodyNode->addChild(handNode);
   expectedHierarchy->addChild(raymanBodyNode);

   NodeHierarchyWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedHierarchy), writer(*node));

   delete node;
   delete expectedHierarchy;
}

///////////////////////////////////////////////////////////////////////////////
