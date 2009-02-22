#include "TestFramework.h"
#include "GraphicalEntityLoaderMock.h"
#include "ResourceManagerStub.h"
#include "Material.h"
#include "MatrixWriter.h"


//////////////////////////////////////////////////////////////////////////////

TEST(EntityLoader, loadSimpleMesh)
{
   ResourceManagerStub rm;

   MeshDefinition mesh;
   mesh.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   mesh.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0));

   GraphicalEntityLoaderMock loader(mesh);
   loader.load(rm, "entity");

   CPPUNIT_ASSERT(rm.isGraphicalEntityRegistered("entity"));
   GraphicalEntity& rootEntity = rm.getGraphicalEntity("entity");
   CPPUNIT_ASSERT_EQUAL(static_cast<UINT> (0), rootEntity.getChildren().size());
}

//////////////////////////////////////////////////////////////////////////////

TEST(EntityLoader, loadMeshWithManyMaterials)
{
   ResourceManagerStub rm;

   MeshDefinition mesh;
   MaterialDefinition mat1;
   mat1.texName = "a.jpg";
   mesh.materials.push_back(mat1);
   mesh.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   mesh.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0));

   MaterialDefinition mat2;
   mat2.texName = "b.jpg";
   MeshDefinition subMesh1;
   subMesh1.materials.push_back(mat2);
   subMesh1.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   subMesh1.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0));
   mesh.children.push_back(subMesh1);

   GraphicalEntityLoaderMock loader(mesh);
   loader.load(rm, "entity");

   CPPUNIT_ASSERT(rm.isGraphicalEntityRegistered("entity"));

   GraphicalEntity& rootEntity = rm.getGraphicalEntity("entity");
   CPPUNIT_ASSERT_EQUAL(static_cast<UINT> (1), rootEntity.getChildren().size());
}

//////////////////////////////////////////////////////////////////////////////

TEST(EntityLoader, loadMeshWithManyRepetitiveMaterials)
{
   ResourceManagerStub rm;

   MeshDefinition mesh;
   MaterialDefinition mat1;
   mat1.texName = "a.jpg";
   mesh.materials.push_back(mat1);
   mesh.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   mesh.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0));

   MaterialDefinition mat2;
   mat2.texName = "b.jpg";
   MeshDefinition subMesh1;
   subMesh1.materials.push_back(mat2);
   subMesh1.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   subMesh1.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0));
   mesh.children.push_back(subMesh1);

   MeshDefinition subMesh2;
   subMesh2.materials.push_back(mat2);
   subMesh2.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   subMesh2.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0));
   mesh.children.push_back(subMesh2);

   MeshDefinition subMesh3;
   subMesh3.materials.push_back(mat1);
   subMesh3.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   subMesh3.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0));
   mesh.children.push_back(subMesh3);

   GraphicalEntityLoaderMock loader(mesh);
   loader.load(rm, "entity");

   CPPUNIT_ASSERT(rm.isGraphicalEntityRegistered("entity"));

   GraphicalEntity& rootEntity = rm.getGraphicalEntity("entity");
   CPPUNIT_ASSERT_EQUAL(static_cast<UINT> (3), rootEntity.getChildren().size());
}

//////////////////////////////////////////////////////////////////////////////

TEST(EntityLoader, hierarchicalEntityWithSpacers)
{
   ResourceManagerStub rm;

   MeshDefinition spacerMesh;
   D3DXMatrixTranslation(&spacerMesh.localMtx, 10, 20, 30);

   MaterialDefinition mat2;
   mat2.texName = "b.jpg";
   MeshDefinition subMesh1;
   subMesh1.materials.push_back(mat2);
   subMesh1.faces.push_back(Face<USHORT>(0, 1, 2, 0));
   subMesh1.vertices.push_back(LitVertex(0, 0, 0, 0, 0, 0, 0, 0));
   spacerMesh.children.push_back(subMesh1);

   GraphicalEntityLoaderMock loader(spacerMesh);
   loader.load(rm, "entity");

   CPPUNIT_ASSERT(rm.isGraphicalEntityRegistered("entity"));

   GraphicalEntity& rootEntity = rm.getGraphicalEntity("entity");
   CPPUNIT_ASSERT_EQUAL(static_cast<UINT> (1), rootEntity.getChildren().size());

   CPPUNIT_ASSERT_EQUAL(static_cast<UINT> (0), rootEntity.getNumSubsets());
   CPPUNIT_ASSERT_EQUAL(spacerMesh.localMtx, rootEntity.getLocalMtx());
}

//////////////////////////////////////////////////////////////////////////////
