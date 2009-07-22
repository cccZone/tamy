#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\SkinnedGraphicalNode.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core\MatrixWriter.h"
#include "GraphicalEntityMock.h"
#include "SkinnedGraphicalEntityMock.h"
#include "core-Renderer\CompositeGraphicalEntity.h"
#include <vector>
#include "NodeHierarchyWriter.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "TransparencyEnablerStub.h"
#include "CoordinatesOperationMock.h"
#include "RenderingTechniqueStub.h"


using namespace RegularTests;

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEntityInstantiation, basics)
{
   std::vector<RenderingTechnique*> techniquesListStub;
   techniquesListStub.push_back(reinterpret_cast<RenderingTechnique*> (0xCC));

   D3DXMATRIX bodyMtx; D3DXMatrixIdentity(&bodyMtx);
   D3DXMATRIX identityMtx; D3DXMatrixIdentity(&identityMtx);
   D3DXMATRIX headMtx; D3DXMatrixTranslation(&headMtx, 0, -5, 0);
   D3DXMATRIX handMtx; D3DXMatrixTranslation(&handMtx, 5, 0, 0);

   // prepare the entities
   CompositeGraphicalEntity bodyRoot("body", bodyMtx);
   GraphicalEntityMock* bodyEntity = new GraphicalEntityMock("body_entity", techniquesListStub);
   CompositeGraphicalEntity* headRoot = new CompositeGraphicalEntity("head", headMtx);
   GraphicalEntityMock* headEntity = new GraphicalEntityMock("head_entity", techniquesListStub);
   CompositeGraphicalEntity* handRoot = new CompositeGraphicalEntity("hand", handMtx);
   GraphicalEntityMock* handEntity = new GraphicalEntityMock("hand_entity", techniquesListStub);

   headRoot->addChild(headEntity);
   handRoot->addChild(handEntity);
   bodyRoot.addChild(bodyEntity);
   bodyRoot.addChild(headRoot);
   bodyRoot.addChild(handRoot);

   GraphicalEntityInstantiator* node = new GraphicalEntityInstantiator("someReference", false);
   node->attachEntity(bodyRoot);

   // prepare the results
   GraphicalEntityInstantiator* expectedHierarchyRoot = new GraphicalEntityInstantiator("someReference", false);

   Node* bodyRootNode = new Node("body", false);
   bodyRootNode->setLocalMtx(bodyMtx);

   Node* headRootNode = new Node("head", false);
   headRootNode->setLocalMtx(headMtx);

   Node* handRootNode = new Node("hand", false);
   handRootNode->setLocalMtx(handMtx);

   Node* bodyNode = new Node("body_entity", false);
   bodyNode->addChild(new GraphicalNode("body_entity_subset0", false, *bodyEntity, 0));

   Node* headNode = new Node("head_entity", false);
   headNode->addChild(new GraphicalNode("head_entity_subset0", false, *headEntity, 0));

   Node* handNode = new Node("hand_entity", false);
   handNode->addChild(new GraphicalNode("hand_entity_subset0", false, *handEntity, 0));

   bodyRootNode->addChild(bodyNode);
   bodyRootNode->addChild(headRootNode);
   bodyRootNode->addChild(handRootNode);
   headRootNode->addChild(headNode);
   handRootNode->addChild(handNode);

   expectedHierarchyRoot->addChild(bodyRootNode);

   NodeHierarchyWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedHierarchyRoot), writer(*node));

   delete node;
   delete expectedHierarchyRoot;
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEntityInstantiation, multipleSubsets)
{
   std::vector<RenderingTechnique*> bodyTechniques;
   std::vector<RenderingTechnique*> headTechniques;
   std::vector<RenderingTechnique*> handTechniques;

   RenderingTechniqueStub skinMat;
   RenderingTechniqueStub blouseMat;
   RenderingTechniqueStub hairMat;

   bodyTechniques.push_back(&skinMat);
   bodyTechniques.push_back(&blouseMat);
   headTechniques.push_back(&skinMat);
   headTechniques.push_back(&hairMat);
   handTechniques.push_back(&skinMat);

   D3DXMATRIX bodyMtx; D3DXMatrixIdentity(&bodyMtx);
   D3DXMATRIX identityMtx; D3DXMatrixIdentity(&identityMtx);
   D3DXMATRIX headMtx; D3DXMatrixTranslation(&headMtx, 0, -5, 0);
   D3DXMATRIX handMtx; D3DXMatrixTranslation(&handMtx, 5, 0, 0);

   // prepare the entities
   CompositeGraphicalEntity bodyRoot("bodyRoot", bodyMtx);
   GraphicalEntityMock* bodyEntity = new GraphicalEntityMock("body_entity", bodyTechniques);
   CompositeGraphicalEntity* headRoot = new CompositeGraphicalEntity("head", headMtx);
   GraphicalEntityMock* headEntity = new GraphicalEntityMock("head_entity", headTechniques);
   CompositeGraphicalEntity* handRoot = new CompositeGraphicalEntity("hand", handMtx);
   GraphicalEntityMock* handEntity = new GraphicalEntityMock("hand_entity", handTechniques);

   headRoot->addChild(headEntity);
   handRoot->addChild(handEntity);
   bodyRoot.addChild(bodyEntity);
   bodyRoot.addChild(headRoot);
   bodyRoot.addChild(handRoot);

   GraphicalEntityInstantiator* node = new GraphicalEntityInstantiator("someReference", false);
   node->attachEntity(bodyRoot);

   // prepare the results
   GraphicalEntityInstantiator* expectedHierarchyRoot = new GraphicalEntityInstantiator("someReference", false);

   Node* bodyRootNode = new Node("bodyRoot", false);
   bodyRootNode->setLocalMtx(bodyMtx);

   Node* headRootNode = new Node("head", false);
   headRootNode->setLocalMtx(headMtx);

   Node* handRootNode = new Node("hand", false);
   handRootNode->setLocalMtx(handMtx);

   Node* bodyNode = new Node("body_entity", false);
   bodyNode->addChild(new GraphicalNode("body_entity_subset0", false, *bodyEntity, 0));
   bodyNode->addChild(new GraphicalNode("body_entity_subset1", false, *bodyEntity, 1));

   Node* headNode = new Node("head_entity", false);
   headNode->addChild(new GraphicalNode("head_entity_subset0", false, *headEntity, 0));
   headNode->addChild(new GraphicalNode("head_entity_subset1", false, *headEntity, 1));

   Node* handNode = new Node("hand_entity", false);
   handNode->addChild(new GraphicalNode("hand_entity_subset0", false, *handEntity, 0));

   bodyRootNode->addChild(bodyNode);
   bodyRootNode->addChild(headRootNode);
   bodyRootNode->addChild(handRootNode);
   headRootNode->addChild(headNode);
   handRootNode->addChild(handNode);
   expectedHierarchyRoot->addChild(bodyRootNode);

   NodeHierarchyWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedHierarchyRoot), writer(*node));

   delete node;
   delete expectedHierarchyRoot;
}

///////////////////////////////////////////////////////////////////////////////

TEST(SkinnedGraphicalEntityInstantiation, basics)
{
   BonesInfluenceDefinition tmpVec;

   D3DXMATRIX bodyMtx; D3DXMatrixIdentity(&bodyMtx);
   D3DXMATRIX bodyBoneMtx; D3DXMatrixTranslation(&bodyBoneMtx, 0, 0, 3);
   D3DXMATRIX headBoneMtx; D3DXMatrixTranslation(&headBoneMtx, 0, -5, 0);
   D3DXMATRIX handBoneMtx; D3DXMatrixTranslation(&handBoneMtx, 5, 0, 0);
   D3DXMATRIX boneOffsetMatrixStub; D3DXMatrixIdentity(&boneOffsetMatrixStub);

   // prepare the entity
   // ... bones structure
   std::vector<SkinBoneDefinition> skeleton;
   skeleton.push_back(SkinBoneDefinition("bodyBone", boneOffsetMatrixStub));
   skeleton.push_back(SkinBoneDefinition("headBone", boneOffsetMatrixStub));
   skeleton.push_back(SkinBoneDefinition("handBone", boneOffsetMatrixStub));

   // ... attributes
   std::vector<BonesInfluenceDefinition> attributes;
   RenderingTechniqueStub renderingTechnique;
   std::vector<RenderingTechnique*> techniques;
   tmpVec.clear(); tmpVec.push_back("bodyBone"); tmpVec.push_back("headBone"); // attribute subset 0
   attributes.push_back(tmpVec);
   techniques.push_back(&renderingTechnique);
   tmpVec.clear(); tmpVec.push_back("headBone");                               // attribute subset 1
   attributes.push_back(tmpVec);
   techniques.push_back(&renderingTechnique);
   tmpVec.clear(); tmpVec.push_back("handBone");                               // attribute subset 2
   attributes.push_back(tmpVec);
   techniques.push_back(&renderingTechnique);

   // ... and finally the entity itself
   SkinnedGraphicalEntityMock body("body", skeleton, attributes, techniques);

   // create the skeletal structure the skin is stretched over
   Node bodyBoneNode("bodyBone", false);
   bodyBoneNode.setLocalMtx(bodyBoneMtx);

   Node* headBoneNode = new Node("headBone", false);
   headBoneNode->setLocalMtx(headBoneMtx);

   Node* handBoneNode = new Node("handBone", false);
   handBoneNode->setLocalMtx(handBoneMtx);

   bodyBoneNode.addChild(headBoneNode);
   bodyBoneNode.addChild(handBoneNode);


   // instantiate the skin
   Node* node = body.instantiate(bodyBoneNode, false);

   // prepare the results
   Node* bodyNode = new Node("body", false);
   bodyNode->setLocalMtx(bodyMtx);

   std::vector<std::pair<Node*, D3DXMATRIX> > bones;
   bones.clear();
   bones.push_back(std::make_pair(&bodyBoneNode, boneOffsetMatrixStub));
   bones.push_back(std::make_pair(headBoneNode, boneOffsetMatrixStub));
   Node* bodySkinNodeSubset0 = new SkinnedGraphicalNode("body_subset0", false, body, 0, bones);

   bones.clear();
   bones.push_back(std::make_pair(headBoneNode, boneOffsetMatrixStub));
   Node* bodySkinNodeSubset1 = new SkinnedGraphicalNode("body_subset1", false, body, 1, bones);

   bones.clear();
   bones.push_back(std::make_pair(handBoneNode, boneOffsetMatrixStub));
   Node* bodySkinNodeSubset2 = new SkinnedGraphicalNode("body_subset2", false, body, 2, bones);

   bodyNode->addChild(bodySkinNodeSubset0);
   bodyNode->addChild(bodySkinNodeSubset1);
   bodyNode->addChild(bodySkinNodeSubset2);

   // compare the hierarchies
   NodeHierarchyWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*bodyNode), writer(*node));

   delete node;
   delete bodyNode;
}

///////////////////////////////////////////////////////////////////////////////

TEST(SkinnedGraphicalEntityInstantiation, bonoeReferenceMissing)
{
   BonesInfluenceDefinition tmpVec;

   D3DXMATRIX bodyMtx; D3DXMatrixIdentity(&bodyMtx);
   D3DXMATRIX bodyBoneMtx; D3DXMatrixTranslation(&bodyBoneMtx, 0, 0, 3);
   D3DXMATRIX headBoneMtx; D3DXMatrixTranslation(&headBoneMtx, 0, -5, 0);
   D3DXMATRIX handBoneMtx; D3DXMatrixTranslation(&handBoneMtx, 5, 0, 0);
   D3DXMATRIX boneOffsetMatrixStub; D3DXMatrixIdentity(&boneOffsetMatrixStub);

   // prepare the entity
   // ... bones structure
   std::vector<SkinBoneDefinition> skeleton;
   skeleton.push_back(SkinBoneDefinition("bodyBone", boneOffsetMatrixStub));
   skeleton.push_back(SkinBoneDefinition("headBone", boneOffsetMatrixStub));
   skeleton.push_back(SkinBoneDefinition("handBone", boneOffsetMatrixStub));

   // ... attributes
   std::vector<BonesInfluenceDefinition> attributes;
   RenderingTechniqueStub renderingTechnique;
   std::vector<RenderingTechnique*> techniques;
   tmpVec.clear(); tmpVec.push_back("bodyBone"); tmpVec.push_back("headBone"); // attribute subset 0
   attributes.push_back(tmpVec);
   techniques.push_back(&renderingTechnique);
   tmpVec.clear(); tmpVec.push_back("headBone");                               // attribute subset 1
   attributes.push_back(tmpVec);
   techniques.push_back(&renderingTechnique);
   tmpVec.clear(); tmpVec.push_back("handBone");                               // attribute subset 2
   attributes.push_back(tmpVec);
   techniques.push_back(&renderingTechnique);

   // ... and finally the entity itself
   SkinnedGraphicalEntityMock body("body", skeleton, attributes, techniques);

   // create the skeletal structure the skin is stretched over
   Node bodyBoneNode("bodyBone", false);
   bodyBoneNode.setLocalMtx(bodyBoneMtx);

   // head bone missing

   Node* handBoneNode = new Node("handBone", false);
   handBoneNode->setLocalMtx(handBoneMtx);

   bodyBoneNode.addChild(handBoneNode);

   // instantiate the skin
   CPPUNIT_ASSERT_THROW(body.instantiate(bodyBoneNode, false), std::runtime_error);
}

///////////////////////////////////////////////////////////////////////////////

TEST(SkinnedGraphicalEntityInstantiation, instantiationViaGraphicalNodeInstantiator)
{
   BonesInfluenceDefinition tmpVec;

   D3DXMATRIX bodyMtx; D3DXMatrixIdentity(&bodyMtx);
   D3DXMATRIX bodyBoneMtx; D3DXMatrixTranslation(&bodyBoneMtx, 0, 0, 3);
   D3DXMATRIX headBoneMtx; D3DXMatrixTranslation(&headBoneMtx, 0, -5, 0);
   D3DXMATRIX handBoneMtx; D3DXMatrixTranslation(&handBoneMtx, 5, 0, 0);
   D3DXMATRIX boneOffsetMatrixStub; D3DXMatrixIdentity(&boneOffsetMatrixStub);

   // prepare the entity
   // ... bones structure
   std::vector<SkinBoneDefinition> skeleton;
   skeleton.push_back(SkinBoneDefinition("bodyBone", boneOffsetMatrixStub));
   skeleton.push_back(SkinBoneDefinition("headBone", boneOffsetMatrixStub));
   skeleton.push_back(SkinBoneDefinition("handBone", boneOffsetMatrixStub));

   // ... attributes
   std::vector<BonesInfluenceDefinition> attributes;
   RenderingTechniqueStub renderingTechnique;
   std::vector<RenderingTechnique*> techniques;
   tmpVec.clear(); tmpVec.push_back("bodyBone"); tmpVec.push_back("headBone"); // attribute subset 0
   attributes.push_back(tmpVec);
   techniques.push_back(&renderingTechnique);
   tmpVec.clear(); tmpVec.push_back("headBone");                               // attribute subset 1
   attributes.push_back(tmpVec);
   techniques.push_back(&renderingTechnique);
   tmpVec.clear(); tmpVec.push_back("handBone");                               // attribute subset 2
   attributes.push_back(tmpVec);
   techniques.push_back(&renderingTechnique);

   // ... and finally the entities themselves
   CompositeGraphicalEntity bodyRoot("bodyRoot", bodyMtx);
   SkinnedGraphicalEntityMock* bodySkin = new SkinnedGraphicalEntityMock("bodySkin", skeleton, attributes, techniques);
   CompositeGraphicalEntity* bodyBone = new CompositeGraphicalEntity("bodyBone", bodyBoneMtx);
   CompositeGraphicalEntity* headBone = new CompositeGraphicalEntity("headBone", headBoneMtx);
   CompositeGraphicalEntity* handBone = new CompositeGraphicalEntity("handBone", handBoneMtx);

   bodyBone->addChild(headBone);
   bodyBone->addChild(handBone);
   bodyRoot.addChild(bodySkin);
   bodyRoot.addChild(bodyBone);

   // instantiate the skin
   GraphicalEntityInstantiator* node = new GraphicalEntityInstantiator("someReference", false);
   node->attachEntity(bodyRoot);

   // prepare the results
   GraphicalEntityInstantiator* expectedHierarchyRoot = new GraphicalEntityInstantiator("someReference", false);

   Node* bodyRootNode = new Node("bodyRoot", false);
   bodyRootNode->setLocalMtx(bodyMtx);

   Node* bodyBoneNode = new Node("bodyBone", false);
   bodyBoneNode->setLocalMtx(bodyBoneMtx);

   Node* headBoneNode = new Node("headBone", false);
   headBoneNode->setLocalMtx(headBoneMtx);

   Node* handBoneNode = new Node("handBone", false);
   handBoneNode->setLocalMtx(handBoneMtx);

   Node* bodySkinNode = new Node("bodySkin", false);

   std::vector<std::pair<Node*, D3DXMATRIX> > bones;
   bones.clear();
   bones.push_back(std::make_pair(bodyBoneNode, boneOffsetMatrixStub));
   bones.push_back(std::make_pair(headBoneNode, boneOffsetMatrixStub));
   Node* bodySkinNodeSubset0 = new SkinnedGraphicalNode("bodySkin_subset0", false, *bodySkin, 0, bones);

   bones.clear();
   bones.push_back(std::make_pair(headBoneNode, boneOffsetMatrixStub));
   Node* bodySkinNodeSubset1 = new SkinnedGraphicalNode("bodySkin_subset1", false, *bodySkin, 1, bones);

   bones.clear();
   bones.push_back(std::make_pair(handBoneNode, boneOffsetMatrixStub));
   Node* bodySkinNodeSubset2 = new SkinnedGraphicalNode("bodySkin_subset2", false, *bodySkin, 2, bones);

   bodyBoneNode->addChild(headBoneNode);
   bodyBoneNode->addChild(handBoneNode);

   bodySkinNode->addChild(bodySkinNodeSubset0);
   bodySkinNode->addChild(bodySkinNodeSubset1);
   bodySkinNode->addChild(bodySkinNodeSubset2);

   bodyRootNode->addChild(bodyBoneNode);
    bodyRootNode->addChild(bodySkinNode);

   expectedHierarchyRoot->addChild(bodyRootNode);

   // compare the hierarchies
   NodeHierarchyWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedHierarchyRoot), writer(*node));

   delete node;
   delete expectedHierarchyRoot;
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEntityInstantiator, attachingManyEntities)
{
   D3DXMATRIX identityMtx; D3DXMatrixIdentity(&identityMtx);

   // prepare the entities
   CompositeGraphicalEntity body("body", identityMtx);
   CompositeGraphicalEntity rifle("rifle", identityMtx);

   NodeHierarchyWriter writer;
   GraphicalEntityInstantiator* node = new GraphicalEntityInstantiator("someReference", false);

   // attach the first entity
   node->attachEntity(body);
   GraphicalEntityInstantiator* expectedHierarchy = new GraphicalEntityInstantiator("someReference", false);
   expectedHierarchy->addChild(new Node("body", false));
   CPPUNIT_ASSERT_EQUAL(writer(*expectedHierarchy), writer(*node));
   delete expectedHierarchy;

   // attach the first second
   node->attachEntity(rifle);
   expectedHierarchy = new GraphicalEntityInstantiator("someReference", false);
   expectedHierarchy->addChild(new Node("rifle", false));
   CPPUNIT_ASSERT_EQUAL(writer(*expectedHierarchy), writer(*node));
   delete expectedHierarchy;

   delete node;
}

///////////////////////////////////////////////////////////////////////////////
