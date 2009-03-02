#include "TestFramework.h"
#include "GraphicalEntityLoaderMock.h"
#include "ResourceManagerStub.h"
#include "Material.h"
#include "MatrixWriter.h"
#include "GraphicalEntityHierarchyWriter.h"
#include "CompositeGraphicalEntity.h"


//////////////////////////////////////////////////////////////////////////////

TEST(EntityLoader, loadSimpleMesh)
{
   GraphicalEntityHierarchyWriter writer;
   ResourceManagerStub rm;
   D3DXMATRIX identityMtx; D3DXMatrixIdentity(&identityMtx);

   MeshDefinition mesh;
   mesh.name = "entityMeshName";
   D3DXMatrixTranslation(&(mesh.localMtx), 1, 2, 3);
   mesh.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   mesh.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

   GraphicalEntityLoaderMock loader(mesh);
   loader.load(rm, "entityRegistrationName");
   CPPUNIT_ASSERT(rm.isGraphicalEntityRegistered("entityRegistrationName"));
   AbstractGraphicalEntity& rootEntity = rm.getGraphicalEntity("entityRegistrationName");

   // prepare the expected results 
   std::vector<Material*> materials;
   CompositeGraphicalEntity expectedHierarchy("entityMeshName", mesh.localMtx);
   GraphicalEntityMock* entity = new GraphicalEntityMock("entityMeshName_entity", materials);
   expectedHierarchy.addChild(entity);

   CPPUNIT_ASSERT_EQUAL(writer(expectedHierarchy), writer(rootEntity));
}

//////////////////////////////////////////////////////////////////////////////

TEST(EntityLoader, loadMeshWithManyMaterials)
{
   GraphicalEntityHierarchyWriter writer;
   ResourceManagerStub rm;
   D3DXMATRIX identityMtx; D3DXMatrixIdentity(&identityMtx);

   MeshDefinition mesh;
   mesh.name = "entity";
   D3DXMatrixTranslation(&(mesh.localMtx), 10, 15, 20);
   MaterialDefinition mat1;
   mat1.texName = "a.jpg";
   mesh.materials.push_back(mat1);
   mesh.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   mesh.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

   MaterialDefinition mat2;
   mat2.texName = "b.jpg";
   MeshDefinition subMesh1;
   subMesh1.name = "subMesh1";
   D3DXMatrixTranslation(&(subMesh1.localMtx), 5, 0, 2);
   subMesh1.materials.push_back(mat2);
   subMesh1.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   subMesh1.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   mesh.children.push_back(subMesh1);

   GraphicalEntityLoaderMock loader(mesh);
   loader.load(rm, "entity");

   CPPUNIT_ASSERT(rm.isGraphicalEntityRegistered("entity"));
   AbstractGraphicalEntity& rootEntity = rm.getGraphicalEntity("entity");

   // prepare the expected results 
   std::vector<Material*> materials;

   CompositeGraphicalEntity expectedHierarchy("entity", mesh.localMtx);
   GraphicalEntityMock* topMeshEntity = new GraphicalEntityMock("entity_entity", materials);

   CompositeGraphicalEntity* subMeshNode = new CompositeGraphicalEntity("subMesh1", subMesh1.localMtx);
   GraphicalEntityMock* subMeshEntity = new GraphicalEntityMock("subMesh1_entity", materials);

   expectedHierarchy.addChild(topMeshEntity);
   expectedHierarchy.addChild(subMeshNode);
   subMeshNode->addChild(subMeshEntity);

   CPPUNIT_ASSERT_EQUAL(writer(expectedHierarchy), writer(rootEntity));
}

//////////////////////////////////////////////////////////////////////////////

TEST(EntityLoader, hierarchicalEntityWithSpacers)
{
   GraphicalEntityHierarchyWriter writer;
   ResourceManagerStub rm;
   D3DXMATRIX identityMtx; D3DXMatrixIdentity(&identityMtx);

   MeshDefinition spacerMesh;
   spacerMesh.name = "entity";
   D3DXMatrixTranslation(&(spacerMesh.localMtx), 10, 20, 30);

   MaterialDefinition mat2;
   mat2.texName = "b.jpg";
   MeshDefinition subMesh1;
   subMesh1.name = "subMesh1";
   D3DXMatrixTranslation(&(subMesh1.localMtx), -15, -10, -5);
   subMesh1.materials.push_back(mat2);
   subMesh1.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   subMesh1.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
   spacerMesh.children.push_back(subMesh1);

   GraphicalEntityLoaderMock loader(spacerMesh);
   loader.load(rm, "entity");

   CPPUNIT_ASSERT(rm.isGraphicalEntityRegistered("entity"));
   AbstractGraphicalEntity& rootEntity = rm.getGraphicalEntity("entity");

   // prepare the expected results 
   std::vector<Material*> materials;

   CompositeGraphicalEntity expectedHierarchy("entity", spacerMesh.localMtx);

   CompositeGraphicalEntity* subMeshNode = new CompositeGraphicalEntity("subMesh1", subMesh1.localMtx);
   GraphicalEntityMock* subMeshEntity = new GraphicalEntityMock("subMesh1_entity", materials);

   expectedHierarchy.addChild(subMeshNode);
   subMeshNode->addChild(subMeshEntity);

   CPPUNIT_ASSERT_EQUAL(writer(expectedHierarchy), writer(rootEntity));
}

//////////////////////////////////////////////////////////////////////////////

TEST(EntityLoader, ensuringNameForUnknamedMeshes)
{
   GraphicalEntityHierarchyWriter writer;
   ResourceManagerStub rm;
   D3DXMATRIX identityMtx; D3DXMatrixIdentity(&identityMtx);

   MeshDefinition spacerMesh;

   spacerMesh.localMtx = identityMtx;

   MeshDefinition subMesh;
   subMesh.localMtx = identityMtx;
   spacerMesh.children.push_back(subMesh);

   GraphicalEntityLoaderMock loader(spacerMesh);
   loader.load(rm, "entity");

   CPPUNIT_ASSERT(rm.isGraphicalEntityRegistered("entity"));
   AbstractGraphicalEntity& rootEntity = rm.getGraphicalEntity("entity");

   // prepare the expected results 
   std::vector<Material*> materials;

   CompositeGraphicalEntity expectedHierarchy("unnamedEntity_0", spacerMesh.localMtx);
   CompositeGraphicalEntity* subMeshNode = new CompositeGraphicalEntity("unnamedEntity_1", subMesh.localMtx);
   expectedHierarchy.addChild(subMeshNode);

   CPPUNIT_ASSERT_EQUAL(writer(expectedHierarchy), writer(rootEntity));
}

//////////////////////////////////////////////////////////////////////////////

TEST(EntityLoader, loadingSkinnedEntity)
{
   GraphicalEntityHierarchyWriter writer;
   ResourceManagerStub rm;
   BonesInfluenceDefinition boneNames;

   // ----------- prepare the entity definition -----------
   // .. materials
   MaterialDefinition mat;
   mat.texName = "b.jpg";

   MeshDefinition mesh;
   mesh.name = "torso";
   mesh.isSkin = true;

   // ... skeletal bones structure
   D3DXMATRIX bodyMtx; D3DXMatrixIdentity(&bodyMtx);
   D3DXMATRIX bodyBoneMtx; D3DXMatrixTranslation(&bodyBoneMtx, 0, 0, 3);
   D3DXMATRIX headBoneMtx; D3DXMatrixTranslation(&headBoneMtx, 0, -5, 0);
   D3DXMATRIX handBoneMtx; D3DXMatrixTranslation(&handBoneMtx, 5, 0, 0);
   D3DXMATRIX boneOffsetMatrixStub; D3DXMatrixIdentity(&boneOffsetMatrixStub);

   mesh.skinBones.push_back(SkinBoneDefinition("bodyBone", boneOffsetMatrixStub));
   mesh.skinBones.push_back(SkinBoneDefinition("headBone", boneOffsetMatrixStub));
   mesh.skinBones.push_back(SkinBoneDefinition("handBone", boneOffsetMatrixStub));

   // ... attrib 0
   boneNames.clear(); boneNames.push_back("bodyBone"); boneNames.push_back("headBone");
   mesh.bonesInfluencingAttribute.push_back(boneNames);
   mesh.materials.push_back(mat);

   // ... attrib 1
   boneNames.clear(); boneNames.push_back("headBone");
   mesh.bonesInfluencingAttribute.push_back(boneNames);
   mesh.materials.push_back(mat);

   // ... attrib 2
   boneNames.clear(); boneNames.push_back("handBone");
   mesh.bonesInfluencingAttribute.push_back(boneNames);
   mesh.materials.push_back(mat);

   // ... geometry
   D3DXMatrixTranslation(&(mesh.localMtx), -15, -10, -5);
   mesh.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   mesh.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

   // ----------- load the hierarchy -----------
   GraphicalEntityLoaderMock loader(mesh);
   loader.load(rm, "entity");

   CPPUNIT_ASSERT(rm.isGraphicalEntityRegistered("entity"));
   AbstractGraphicalEntity& rootEntity = rm.getGraphicalEntity("entity");

   //  ----------- prepare the expected results -----------
   SkinBoneDefinition skeleton;
   std::vector<BonesInfluenceDefinition> attributes;
   std::vector<Material*> materials;

   // attrib 0
   boneNames.clear(); boneNames.push_back("bodyBone"); boneNames.push_back("headBone");
   attributes.push_back(boneNames);
   materials.push_back(reinterpret_cast<Material*> (NULL));

   // attrib 1
   boneNames.clear(); boneNames.push_back("headBone");
   attributes.push_back(boneNames);
   materials.push_back(reinterpret_cast<Material*> (NULL));

   // attrib 2
   boneNames.clear(); boneNames.push_back("handBone");
   attributes.push_back(boneNames);
   materials.push_back(reinterpret_cast<Material*> (NULL));

   CompositeGraphicalEntity expectedHierarchy("torso", mesh.localMtx);
   SkinnedGraphicalEntityMock* subMeshNode = new SkinnedGraphicalEntityMock("torso_entity", 
                                                                            mesh.skinBones, 
                                                                            mesh.bonesInfluencingAttribute, 
                                                                            materials);
   expectedHierarchy.addChild(subMeshNode);

   CPPUNIT_ASSERT_EQUAL(writer(expectedHierarchy), writer(rootEntity));
}

//////////////////////////////////////////////////////////////////////////////
