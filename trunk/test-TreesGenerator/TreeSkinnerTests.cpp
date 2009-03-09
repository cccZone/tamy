#include "TestFramework.h"
#include "TreeSegment.h"
#include "MeshDefinition.h"
#include "MeshDefinitionWriter.h"
#include "TreeSkinner.h"
#include <d3dx9.h>


//////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, skinningSingleSegmentTree)
{
   MaterialDefinition matStub;

   // prepare the tree structure
   TreeSegment structure;
   structure.direction = D3DXVECTOR3(0, 1, 0);
   structure.position = D3DXVECTOR3(0, 0, 0);
   structure.width = 10;
   structure.type = BRANCH_START;
   structure.segmentIdx = 0;

   TreeSegment* child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 1;
   structure.children.push_back(child);

   // create the skin for the tree
   TreeSkinner skinner(structure);
   MeshDefinition mesh;
   unsigned int circRes = 4;
   unsigned int boneRes = 10;
   skinner("tree", circRes, boneRes, matStub, mesh);
   
   // prepare the results
   MeshDefinition expectedMesh;
   expectedMesh.isSkin = true;
   expectedMesh.name = "tree";
   D3DXMatrixIdentity(&expectedMesh.localMtx);
                                            /* coords *//* weights *//* normals */ /* tex coords */
   expectedMesh.vertices.push_back(LitVertex( 5, 0,  0,   1, 0, 0,     1, 0,  0,      0, 0));
   expectedMesh.vertices.push_back(LitVertex( 0, 0,  5,   1, 0, 0,     0, 0,  1,      0.25f, 0));
   expectedMesh.vertices.push_back(LitVertex(-5, 0,  0,   1, 0, 0,    -1, 0,  0,      0.5f, 0));
   expectedMesh.vertices.push_back(LitVertex(0,  0, -5,   1, 0, 0,     0, 0, -1,      0.75f, 0));
   expectedMesh.vertices.push_back(LitVertex(5,  0,  0,   1, 0, 0,     1, 0,  0,      1.f, 0));
   expectedMesh.vertices.push_back(LitVertex(0, 10,  0,   1, 0, 0,     0, 1,  0,      0.5f, 1));
   expectedMesh.faces.push_back(Face<USHORT> (0, 5, 1, 0));
   expectedMesh.faces.push_back(Face<USHORT> (1, 5, 2, 0));
   expectedMesh.faces.push_back(Face<USHORT> (2, 5, 3, 0));
   expectedMesh.faces.push_back(Face<USHORT> (3, 5, 4, 0));
   expectedMesh.faces.push_back(Face<USHORT> (4, 5, 0, 0));

   BonesInfluenceDefinition bonesForAttrib; bonesForAttrib.push_back("treeBone_0");
   expectedMesh.bonesInfluencingAttribute.push_back(bonesForAttrib);

   D3DXMATRIX boneOffsetMtx; D3DXMatrixIdentity(&boneOffsetMtx);
   expectedMesh.skinBones.push_back(SkinBoneDefinition("treeBone_0", boneOffsetMtx));

   MeshDefinition* treeBone_0_Mesh = new MeshDefinition();
   treeBone_0_Mesh->name = "treeBone_0";
   treeBone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBone_0_Mesh->localMtx));
   expectedMesh.children.push_back(treeBone_0_Mesh);

   MeshDefinitionWriter writer;
   writer.writeBasics(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
   writer.writeVertices(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
   writer.writeFaces(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
   writer.writeBones(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, skinningStraightTwoSegmentTree_vertices)
{
   MaterialDefinition matStub;

   // prepare the tree structure
   TreeSegment structure;
   structure.direction = D3DXVECTOR3(0, 1, 0);
   structure.position = D3DXVECTOR3(0, 0, 0);
   structure.width = 10;
   structure.type = BRANCH_START;
   structure.segmentIdx = 0;
   TreeSegment* parentSeg = &structure;

   TreeSegment* child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 5;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parentSeg->children.push_back(child);
   parentSeg = child;

   child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 2;
   parentSeg->children.push_back(child);


   // create the skin for the tree
   TreeSkinner skinner(structure);
   MeshDefinition mesh;
   unsigned int circRes = 4;
   unsigned int boneRes = 10;
   skinner("tree", circRes, boneRes, matStub, mesh);
   
   // prepare the results
   MeshDefinition expectedMesh;
   expectedMesh.isSkin = true;
   expectedMesh.name = "tree";
   D3DXMatrixIdentity(&expectedMesh.localMtx);
                                            /*      coords    *//* weights *//* normals */ /* tex coords */
   expectedMesh.vertices.push_back(LitVertex(   5 ,  0,    0 ,   1, 0, 0,     1, 0,  0,     0,        0));
   expectedMesh.vertices.push_back(LitVertex(   0 ,  0,    5 ,   1, 0, 0,     0, 0,  1,     0.25f,    0));
   expectedMesh.vertices.push_back(LitVertex(  -5 ,  0,    0 ,   1, 0, 0,    -1, 0,  0,     0.5f,     0));
   expectedMesh.vertices.push_back(LitVertex(   0 ,  0,   -5 ,   1, 0, 0,     0, 0, -1,     0.75f,    0));
   expectedMesh.vertices.push_back(LitVertex(   5 ,  0,    0 ,   1, 0, 0,     1, 0,  0,     1.f,      0));
   expectedMesh.vertices.push_back(LitVertex( 2.5f, 10,    0 ,   1, 0, 0,     1, 0,  0,     0,        0.5f));
   expectedMesh.vertices.push_back(LitVertex(   0 , 10,  2.5f,   1, 0, 0,     0, 0,  1,     0.25f,    0.5f));
   expectedMesh.vertices.push_back(LitVertex(-2.5f, 10,    0 ,   1, 0, 0,    -1, 0,  0,     0.5f,     0.5f));
   expectedMesh.vertices.push_back(LitVertex(   0 , 10, -2.5f,   1, 0, 0,     0, 0, -1,     0.75f,    0.5f));
   expectedMesh.vertices.push_back(LitVertex( 2.5f, 10,    0 ,   1, 0, 0,     1, 0,  0,     1.f,      0.5f));
   expectedMesh.vertices.push_back(LitVertex(   0 , 20,    0 ,   1, 0, 0,     0, 1,  0,     0.5f,     1));
   // first seg cylinder shaped faces
   expectedMesh.faces.push_back(Face<USHORT> (0, 5, 1, 0));
   expectedMesh.faces.push_back(Face<USHORT> (1, 5, 6, 0));
   expectedMesh.faces.push_back(Face<USHORT> (1, 6, 2, 0));
   expectedMesh.faces.push_back(Face<USHORT> (2, 6, 7, 0));
   expectedMesh.faces.push_back(Face<USHORT> (2, 7, 3, 0));
   expectedMesh.faces.push_back(Face<USHORT> (3, 7, 8, 0));
   expectedMesh.faces.push_back(Face<USHORT> (3, 8, 4, 0));
   expectedMesh.faces.push_back(Face<USHORT> (4, 8, 9, 0));
   expectedMesh.faces.push_back(Face<USHORT> (4, 9, 0, 0));
   expectedMesh.faces.push_back(Face<USHORT> (0, 9, 5, 0));
   // cap faces
   expectedMesh.faces.push_back(Face<USHORT> (5, 10, 6, 0));
   expectedMesh.faces.push_back(Face<USHORT> (6, 10, 7, 0));
   expectedMesh.faces.push_back(Face<USHORT> (7, 10, 8, 0));
   expectedMesh.faces.push_back(Face<USHORT> (8, 10, 9, 0));
   expectedMesh.faces.push_back(Face<USHORT> (9, 10, 5, 0));

   MeshDefinition* treeBone_0_Mesh = new MeshDefinition();
   treeBone_0_Mesh->name = "treeBone_0";
   treeBone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBone_0_Mesh->localMtx));
   expectedMesh.children.push_back(treeBone_0_Mesh);

   MeshDefinitionWriter writer;
   writer.writeBasics(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
   writer.writeVertices(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
   writer.writeFaces(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, skinningStraightTwoSegmentTree_bones)
{
   MaterialDefinition matStub;

   // prepare the tree structure
   TreeSegment structure;
   structure.direction = D3DXVECTOR3(0, 1, 0);
   structure.position = D3DXVECTOR3(0, 0, 0);
   structure.width = 10;
   structure.type = BRANCH_START;
   structure.segmentIdx = 0;
   TreeSegment* parentSeg = &structure;

   TreeSegment* child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 5;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parentSeg->children.push_back(child);
   parentSeg = child;

   child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 2;
   parentSeg->children.push_back(child);

   // create the skin for the tree
   TreeSkinner skinner(structure);
   MeshDefinition mesh;
   unsigned int circRes = 4;
   unsigned int boneRes = 1;
   skinner("tree", circRes, boneRes, matStub, mesh);
   
   // prepare the results
   MeshDefinition expectedMesh;
   expectedMesh.isSkin = true;
   expectedMesh.name = "tree";
   D3DXMatrixIdentity(&expectedMesh.localMtx);

   BonesInfluenceDefinition bonesForAttrib; 
   bonesForAttrib.push_back("treeBone_0");
   bonesForAttrib.push_back("treeBone_1");
   expectedMesh.bonesInfluencingAttribute.push_back(bonesForAttrib);

   D3DXMATRIX bone0_OffsetMtx; 
   D3DXMatrixIdentity(&bone0_OffsetMtx);

   D3DXMATRIX bone1_OffsetMtx; 
   D3DXMatrixIdentity(&bone1_OffsetMtx);
   bone1_OffsetMtx._42 = -10;
   expectedMesh.skinBones.push_back(SkinBoneDefinition("treeBone_0", bone0_OffsetMtx));
   expectedMesh.skinBones.push_back(SkinBoneDefinition("treeBone_1", bone1_OffsetMtx));
   
   MeshDefinition* treeBone_0_Mesh = new MeshDefinition();
   treeBone_0_Mesh->name = "treeBone_0";
   treeBone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBone_0_Mesh->localMtx));
   expectedMesh.children.push_back(treeBone_0_Mesh);

   MeshDefinition* treeBone_1_Mesh = new MeshDefinition();
   treeBone_1_Mesh->name = "treeBone_1";
   treeBone_1_Mesh->isSkin = false;

   D3DXMATRIX mtxCombined = treeBone_0_Mesh->localMtx;
   D3DXMatrixIdentity(&(treeBone_1_Mesh->localMtx));
   treeBone_1_Mesh->localMtx._41 = 0; treeBone_1_Mesh->localMtx._42 = 10; treeBone_1_Mesh->localMtx._43 = 0;

   treeBone_0_Mesh->children.push_back(treeBone_1_Mesh);

   MeshDefinitionWriter writer;
   writer.writeBasics(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
   writer.writeBones(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, skinningStraightThreeSegmentTreeWithBend_vertices)
{
   MaterialDefinition matStub;

   // prepare the tree structure
   TreeSegment structure;
   structure.rightVec = D3DXVECTOR3(1, 0, 0);
   structure.direction = D3DXVECTOR3(0, 1, 0);
   structure.position = D3DXVECTOR3(0, 0, 0);
   structure.width = 10;
   structure.type = BRANCH_START;
   structure.segmentIdx = 0;
   TreeSegment* parentSeg = &structure;

   TreeSegment* child = new TreeSegment();
   child->rightVec = D3DXVECTOR3(0.707f, 0.707f, 0); // 45 deg around OZ axis
   child->direction = D3DXVECTOR3(-0.707f, 0.707f, 0);
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 5;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parentSeg->children.push_back(child);
   parentSeg = child;

   child = new TreeSegment();
   child->rightVec = D3DXVECTOR3(1, 0, 0);  // 0 deg around OZ axis
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 10;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 2;
   parentSeg->children.push_back(child);
   parentSeg = child;

   child = new TreeSegment();
   child->rightVec = D3DXVECTOR3(0.866f, 0.5f, 0); // 30 deg around OZ axis
   child->direction = D3DXVECTOR3(-0.5f, 0.866f, 0);
   child->position = D3DXVECTOR3(0, 30, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 3;
   parentSeg->children.push_back(child);

   // create the skin for the tree
   TreeSkinner skinner(structure);
   MeshDefinition mesh;
   unsigned int circRes = 4;
   unsigned int boneRes = 10;
   skinner("tree", circRes, boneRes, matStub, mesh);
   
   // prepare the results
   MeshDefinition expectedMesh;
   expectedMesh.isSkin = true;
   expectedMesh.name = "tree";
   D3DXMatrixIdentity(&expectedMesh.localMtx);

                                            /*      coords                 *//* weights *//*        normals        */ /* tex coords */
   // seg 0
   expectedMesh.vertices.push_back(LitVertex(       5 ,        0 ,    0 ,       1, 0, 0,         1 ,        0 ,  0,     0,        0));
   expectedMesh.vertices.push_back(LitVertex(       0 ,        0 ,    5 ,       1, 0, 0,         0 ,        0 ,  1,     0.25f,    0));
   expectedMesh.vertices.push_back(LitVertex(      -5 ,        0 ,    0 ,       1, 0, 0,        -1 ,        0 ,  0,     0.5f,     0));
   expectedMesh.vertices.push_back(LitVertex(       0 ,        0 ,   -5 ,       1, 0, 0,         0 ,        0 , -1,     0.75f,    0));
   expectedMesh.vertices.push_back(LitVertex(       5 ,        0 ,    0 ,       1, 0, 0,         1 ,        0 ,  0,     1.f,      0));
   // seg 1
   expectedMesh.vertices.push_back(LitVertex(  1.7675f,  11.7675f,    0 ,       1, 0, 0,  0.707107f, 0.707107f,  0,     0,        0.333333f));
   expectedMesh.vertices.push_back(LitVertex(       0 ,       10 ,  2.5f,       1, 0, 0,         0 ,        0 ,  1,     0.25f,    0.333333f));
   expectedMesh.vertices.push_back(LitVertex( -1.7675f,   8.2325f,    0 ,       1, 0, 0, -0.707107f,-0.707107f,  0,     0.5f,     0.333333f));
   expectedMesh.vertices.push_back(LitVertex(       0 ,       10 , -2.5f,       1, 0, 0,         0 ,        0 , -1,     0.75f,    0.333333f));
   expectedMesh.vertices.push_back(LitVertex(  1.7675f,  11.7675f,    0 ,       1, 0, 0,  0.707107f, 0.707107f,  0,     1.f,      0.333333f));
   // seg 2
   expectedMesh.vertices.push_back(LitVertex(       5 ,       20 ,    0 ,       1, 0, 0,         1 ,        0 ,  0,     0,        0.666667f));
   expectedMesh.vertices.push_back(LitVertex(       0 ,       20 ,    5 ,       1, 0, 0,         0 ,        0 ,  1,     0.25f,    0.666667f));
   expectedMesh.vertices.push_back(LitVertex(      -5 ,       20 ,    0 ,       1, 0, 0,        -1 ,        0 ,  0,     0.5f,     0.666667f));
   expectedMesh.vertices.push_back(LitVertex(       0 ,       20 ,   -5 ,       1, 0, 0,         0 ,        0 , -1,     0.75f,    0.666667f));
   expectedMesh.vertices.push_back(LitVertex(       5 ,       20 ,    0 ,       1, 0, 0,         1 ,        0 ,  0,     1.f,      0.666667f));
   // end cap
   expectedMesh.vertices.push_back(LitVertex(       0 ,       30 ,    0 ,       1, 0, 0, -0.500011f, 0.866019f,  0,     0.5f,     1));
   // first seg cylinder shaped faces
   expectedMesh.faces.push_back(Face<USHORT> (0, 5, 1, 0));
   expectedMesh.faces.push_back(Face<USHORT> (1, 5, 6, 0));
   expectedMesh.faces.push_back(Face<USHORT> (1, 6, 2, 0));
   expectedMesh.faces.push_back(Face<USHORT> (2, 6, 7, 0));
   expectedMesh.faces.push_back(Face<USHORT> (2, 7, 3, 0));
   expectedMesh.faces.push_back(Face<USHORT> (3, 7, 8, 0));
   expectedMesh.faces.push_back(Face<USHORT> (3, 8, 4, 0));
   expectedMesh.faces.push_back(Face<USHORT> (4, 8, 9, 0));
   expectedMesh.faces.push_back(Face<USHORT> (4, 9, 0, 0));
   expectedMesh.faces.push_back(Face<USHORT> (0, 9, 5, 0));
   // second seg cylinder shaped faces
   expectedMesh.faces.push_back(Face<USHORT> (5, 10, 6, 0));
   expectedMesh.faces.push_back(Face<USHORT> (6, 10, 11, 0));
   expectedMesh.faces.push_back(Face<USHORT> (6, 11, 7, 0));
   expectedMesh.faces.push_back(Face<USHORT> (7, 11, 12, 0));
   expectedMesh.faces.push_back(Face<USHORT> (7, 12, 8, 0));
   expectedMesh.faces.push_back(Face<USHORT> (8, 12, 13, 0));
   expectedMesh.faces.push_back(Face<USHORT> (8, 13, 9, 0));
   expectedMesh.faces.push_back(Face<USHORT> (9, 13, 14, 0));
   expectedMesh.faces.push_back(Face<USHORT> (9, 14, 5, 0));
   expectedMesh.faces.push_back(Face<USHORT> (5, 14, 10, 0));
   // cap faces
   expectedMesh.faces.push_back(Face<USHORT> (10, 15, 11, 0));
   expectedMesh.faces.push_back(Face<USHORT> (11, 15, 12, 0));
   expectedMesh.faces.push_back(Face<USHORT> (12, 15, 13, 0));
   expectedMesh.faces.push_back(Face<USHORT> (13, 15, 14, 0));
   expectedMesh.faces.push_back(Face<USHORT> (14, 15, 10, 0));

   MeshDefinition* treeBone_0_Mesh = new MeshDefinition();
   treeBone_0_Mesh->name = "treeBone_0";
   treeBone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBone_0_Mesh->localMtx));
   expectedMesh.children.push_back(treeBone_0_Mesh);


   MeshDefinitionWriter writer;
   writer.writeBasics(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
   writer.writeVertices(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
   writer.writeFaces(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, skinningStraightThreeSegmentTreeWithBend_matrices)
{
   MaterialDefinition matStub;

   // prepare the tree structure
   TreeSegment structure;
   structure.rightVec = D3DXVECTOR3(1, 0, 0);
   structure.direction = D3DXVECTOR3(0, 1, 0);
   structure.position = D3DXVECTOR3(0, 0, 0);
   structure.width = 10;
   structure.type = BRANCH_START;
   structure.segmentIdx = 0;
   TreeSegment* parentSeg = &structure;

   TreeSegment* child = new TreeSegment();
   float angle = D3DXToRadian(45); // 45 deg around OZ axis
   child->rightVec = D3DXVECTOR3(cos(angle), sin(angle), 0);
   child->direction = D3DXVECTOR3(-sin(angle), cos(angle), 0);
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 5;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parentSeg->children.push_back(child);
   parentSeg = child;

   child = new TreeSegment();
   child->rightVec = D3DXVECTOR3(1, 0, 0);  // 0 deg around OZ axis
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 10;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 2;
   parentSeg->children.push_back(child);
   parentSeg = child;

   child = new TreeSegment();
   angle = D3DXToRadian(30); // 30 deg around OZ axis
   child->rightVec = D3DXVECTOR3(cos(angle), sin(angle), 0);
   child->direction = D3DXVECTOR3(-sin(angle), cos(angle), 0);
   child->position = D3DXVECTOR3(0, 30, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 3;
   parentSeg->children.push_back(child);

   // create the skin for the tree
   TreeSkinner skinner(structure);
   MeshDefinition mesh;
   unsigned int circRes = 4;
   unsigned int boneRes = 1;
   skinner("tree", circRes, boneRes, matStub, mesh);
   
   // prepare the results
   D3DXMATRIX mtxCombined;
   D3DXMATRIX mtxRot;
   D3DXVECTOR3 zVec(0, 0, 1);

   // ... meshes
   MeshDefinition expectedMesh;
   expectedMesh.isSkin = true;
   expectedMesh.name = "tree";
   D3DXMatrixIdentity(&expectedMesh.localMtx);

   MeshDefinition* treeBone_0_Mesh = new MeshDefinition();
   treeBone_0_Mesh->name = "treeBone_0";
   treeBone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBone_0_Mesh->localMtx));
   D3DXMatrixMultiply(&mtxCombined, &mtxCombined, &(treeBone_0_Mesh->localMtx));
   expectedMesh.children.push_back(treeBone_0_Mesh);

   MeshDefinition* treeBone_1_Mesh = new MeshDefinition();
   treeBone_1_Mesh->name = "treeBone_1";
   treeBone_1_Mesh->isSkin = false;
   D3DXMatrixRotationAxis(&mtxRot, &zVec, D3DXToRadian(45));
   D3DXMatrixTranslation(&(treeBone_1_Mesh->localMtx), 0, 10, 0);
   D3DXMatrixMultiply(&(treeBone_1_Mesh->localMtx), &mtxRot, &(treeBone_1_Mesh->localMtx));
   D3DXMatrixMultiply(&mtxCombined, &(treeBone_1_Mesh->localMtx), &mtxCombined);
   treeBone_0_Mesh->children.push_back(treeBone_1_Mesh);

   MeshDefinition* treeBone_2_Mesh = new MeshDefinition();
   treeBone_2_Mesh->name = "treeBone_2";
   treeBone_2_Mesh->isSkin = false;
   D3DXMatrixRotationAxis(&mtxRot, &zVec, D3DXToRadian(-45));
   D3DXMatrixTranslation(&(treeBone_2_Mesh->localMtx), 7.07107f, 7.07107f, 0);
   D3DXMatrixMultiply(&(treeBone_2_Mesh->localMtx), &mtxRot, &(treeBone_2_Mesh->localMtx));
   D3DXMatrixMultiply(&mtxCombined, &(treeBone_2_Mesh->localMtx), &mtxCombined);
   treeBone_1_Mesh->children.push_back(treeBone_2_Mesh);

   // ... bones
   BonesInfluenceDefinition bonesForAttrib; 
   bonesForAttrib.push_back("treeBone_0");
   bonesForAttrib.push_back("treeBone_1");
   bonesForAttrib.push_back("treeBone_2");
   expectedMesh.bonesInfluencingAttribute.push_back(bonesForAttrib);

   D3DXMATRIX currOffsetMtx; D3DXMatrixIdentity(&currOffsetMtx);
   D3DXMATRIX boneOffsetMtx;
   D3DXMatrixMultiply(&currOffsetMtx, &(treeBone_0_Mesh->localMtx), &currOffsetMtx);
   D3DXMatrixInverse(&boneOffsetMtx, NULL, &currOffsetMtx);
   expectedMesh.skinBones.push_back(SkinBoneDefinition("treeBone_0", boneOffsetMtx));

   D3DXMatrixMultiply(&currOffsetMtx, &(treeBone_1_Mesh->localMtx), &currOffsetMtx);
   D3DXMatrixInverse(&boneOffsetMtx, NULL, &currOffsetMtx);
   expectedMesh.skinBones.push_back(SkinBoneDefinition("treeBone_1", boneOffsetMtx));

   D3DXMatrixMultiply(&currOffsetMtx, &(treeBone_2_Mesh->localMtx), &currOffsetMtx);
   D3DXMatrixInverse(&boneOffsetMtx, NULL, &currOffsetMtx); boneOffsetMtx._41 = 0;
   expectedMesh.skinBones.push_back(SkinBoneDefinition("treeBone_2", boneOffsetMtx));

   MeshDefinitionWriter writer;
   writer.writeBasics(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
   writer.writeBones(); CPPUNIT_ASSERT_EQUAL(writer(expectedMesh), writer(mesh));
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, treeWithBranch_vertices)
{
   MaterialDefinition matStub;

   // prepare the tree structure
   TreeSegment structure;
   structure.direction = D3DXVECTOR3(0, 1, 0);
   structure.position = D3DXVECTOR3(0, 0, 0);
   structure.width = 10;
   structure.type = BRANCH_START;
   structure.segmentIdx = 0;
   TreeSegment* parentSeg = &structure;

   TreeSegment* child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 5;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parentSeg->children.push_back(child);
   parentSeg = child;

   {
      float angle = D3DXToRadian(45); // 45 deg around OZ axis
      float c45 = cos(angle);
      float s45 = sin(angle);

      TreeSegment* branchRoot = new TreeSegment();
      branchRoot->rightVec = D3DXVECTOR3(c45, s45, 0);
      branchRoot->direction = D3DXVECTOR3(-s45, c45, 0);
      branchRoot->position = D3DXVECTOR3(0, 10, 0);
      branchRoot->width = 10;
      branchRoot->type = BRANCH_START;
      branchRoot->segmentIdx = 1;
      parentSeg->children.push_back(branchRoot);
      TreeSegment* branchParent = branchRoot;

      TreeSegment* branchChild = new TreeSegment();
      branchChild->rightVec = D3DXVECTOR3(c45, s45, 0);
      branchChild->direction = D3DXVECTOR3(-s45, c45, 0);
      branchChild->position = D3DXVECTOR3(-7.0710669f, 17.071068f, 0);
      branchChild->width = 5;
      branchChild->type = BRANCH_MIDDLE;
      branchChild->segmentIdx = 2;
      branchParent->children.push_back(branchChild);
      branchParent = branchChild;

      branchChild = new TreeSegment();
      branchChild->rightVec = D3DXVECTOR3(c45, s45, 0);
      branchChild->direction = D3DXVECTOR3(-s45, c45, 0);
      branchChild->position = D3DXVECTOR3(-14.142134f, 24.142136f, 0);
      branchChild->width = 0;
      branchChild->type = BRANCH_END;
      branchChild->segmentIdx = 3;
      branchParent->children.push_back(branchChild);
   }

   child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 2;
   parentSeg->children.push_back(child);


   // create the skin for the tree
   TreeSkinner skinner(structure);
   MeshDefinition mesh;
   unsigned int circRes = 4;
   unsigned int boneRes = 10;
   skinner("tree", circRes, boneRes, matStub, mesh);
   
   // prepare the results
   // ... the root
   MeshDefinition expectedRootMesh;
   expectedRootMesh.isSkin = true;
   expectedRootMesh.name = "tree";
   D3DXMatrixIdentity(&expectedRootMesh.localMtx);
                                                /*      coords    *//* weights *//* normals */ /* tex coords */
   expectedRootMesh.vertices.push_back(LitVertex(   5 ,  0,    0 ,   1, 0, 0,     1, 0,  0,     0,        0));
   expectedRootMesh.vertices.push_back(LitVertex(   0 ,  0,    5 ,   1, 0, 0,     0, 0,  1,     0.25f,    0));
   expectedRootMesh.vertices.push_back(LitVertex(  -5 ,  0,    0 ,   1, 0, 0,    -1, 0,  0,     0.5f,     0));
   expectedRootMesh.vertices.push_back(LitVertex(   0 ,  0,   -5 ,   1, 0, 0,     0, 0, -1,     0.75f,    0));
   expectedRootMesh.vertices.push_back(LitVertex(   5 ,  0,    0 ,   1, 0, 0,     1, 0,  0,     1.f,      0));
   expectedRootMesh.vertices.push_back(LitVertex( 2.5f, 10,    0 ,   1, 0, 0,     1, 0,  0,     0,        0.333333f));
   expectedRootMesh.vertices.push_back(LitVertex(   0 , 10,  2.5f,   1, 0, 0,     0, 0,  1,     0.25f,    0.333333f));
   expectedRootMesh.vertices.push_back(LitVertex(-2.5f, 10,    0 ,   1, 0, 0,    -1, 0,  0,     0.5f,     0.333333f));
   expectedRootMesh.vertices.push_back(LitVertex(   0 , 10, -2.5f,   1, 0, 0,     0, 0, -1,     0.75f,    0.333333f));
   expectedRootMesh.vertices.push_back(LitVertex( 2.5f, 10,    0 ,   1, 0, 0,     1, 0,  0,     1.f,      0.333333f));
   expectedRootMesh.vertices.push_back(LitVertex(   0 , 20,    0 ,   1, 0, 0,     0, 1,  0,     0.5f,     0.666667f));
   // first seg cylinder shaped faces
   expectedRootMesh.faces.push_back(Face<USHORT> (0, 5, 1, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (1, 5, 6, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (1, 6, 2, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (2, 6, 7, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (2, 7, 3, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (3, 7, 8, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (3, 8, 4, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (4, 8, 9, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (4, 9, 0, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (0, 9, 5, 0));
   // cap faces
   expectedRootMesh.faces.push_back(Face<USHORT> (5, 10, 6, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (6, 10, 7, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (7, 10, 8, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (8, 10, 9, 0));
   expectedRootMesh.faces.push_back(Face<USHORT> (9, 10, 5, 0));

   MeshDefinition* treeBone_0_Mesh = new MeshDefinition();
   treeBone_0_Mesh->name = "treeBone_0";
   treeBone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBone_0_Mesh->localMtx));
   expectedRootMesh.children.push_back(treeBone_0_Mesh);

   // ... the branch
   MeshDefinition* expectedBranchMesh = new MeshDefinition();
   expectedBranchMesh->isSkin = true;
   expectedBranchMesh->name = "tree_branch_0";

   D3DXMATRIX rotMtx;
   D3DXVECTOR3 zVec(0, 0, 1);
   D3DXMatrixRotationAxis(&rotMtx, &zVec, D3DXToRadian(45));
   D3DXMatrixTranslation(&(expectedBranchMesh->localMtx), 0, 10, 0);
   D3DXMatrixMultiply(&(expectedBranchMesh->localMtx), &rotMtx, &(expectedBranchMesh->localMtx));
   expectedRootMesh.children.push_back(expectedBranchMesh);

                                                    /*      coords    *//* weights *//* normals */ /* tex coords */
   expectedBranchMesh->vertices.push_back(LitVertex(   5 ,  0,    0 ,   1, 0, 0,     1, 0,  0,     0,        0.333333f));
   expectedBranchMesh->vertices.push_back(LitVertex(   0 ,  0,    5 ,   1, 0, 0,     0, 0,  1,     0.25f,    0.333333f));
   expectedBranchMesh->vertices.push_back(LitVertex(  -5 ,  0,    0 ,   1, 0, 0,    -1, 0,  0,     0.5f,     0.333333f));
   expectedBranchMesh->vertices.push_back(LitVertex(   0 ,  0,   -5 ,   1, 0, 0,     0, 0, -1,     0.75f,    0.333333f));
   expectedBranchMesh->vertices.push_back(LitVertex(   5 ,  0,    0 ,   1, 0, 0,     1, 0,  0,     1.f,      0.333333f));
   expectedBranchMesh->vertices.push_back(LitVertex( 2.5f, 10,    0 ,   1, 0, 0,     1, 0,  0,     0,        0.666667f));
   expectedBranchMesh->vertices.push_back(LitVertex(   0 , 10,  2.5f,   1, 0, 0,     0, 0,  1,     0.25f,    0.666667f));
   expectedBranchMesh->vertices.push_back(LitVertex(-2.5f, 10,    0 ,   1, 0, 0,    -1, 0,  0,     0.5f,     0.666667f));
   expectedBranchMesh->vertices.push_back(LitVertex(   0 , 10, -2.5f,   1, 0, 0,     0, 0, -1,     0.75f,    0.666667f));
   expectedBranchMesh->vertices.push_back(LitVertex( 2.5f, 10,    0 ,   1, 0, 0,     1, 0,  0,     1.f,      0.666667f));
   expectedBranchMesh->vertices.push_back(LitVertex(   0 , 20,    0 ,   1, 0, 0,     0, 1,  0,     0.5f,     1));
   // first seg cylinder shaped faces
   expectedBranchMesh->faces.push_back(Face<USHORT> (0, 5, 1, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (1, 5, 6, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (1, 6, 2, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (2, 6, 7, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (2, 7, 3, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (3, 7, 8, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (3, 8, 4, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (4, 8, 9, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (4, 9, 0, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (0, 9, 5, 0));
   // cap faces
   expectedBranchMesh->faces.push_back(Face<USHORT> (5, 10, 6, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (6, 10, 7, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (7, 10, 8, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (8, 10, 9, 0));
   expectedBranchMesh->faces.push_back(Face<USHORT> (9, 10, 5, 0));

   MeshDefinition* treeBranch_0_Bone_0_Mesh = new MeshDefinition();
   treeBranch_0_Bone_0_Mesh->name = "tree_branch_0Bone_0";
   treeBranch_0_Bone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBranch_0_Bone_0_Mesh->localMtx));
   expectedBranchMesh->children.push_back(treeBranch_0_Bone_0_Mesh);

   MeshDefinitionWriter writer;
   writer.writeBasics(); CPPUNIT_ASSERT_EQUAL(writer(expectedRootMesh), writer(mesh));
   writer.writeVertices(); CPPUNIT_ASSERT_EQUAL(writer(expectedRootMesh), writer(mesh));
   writer.writeFaces(); CPPUNIT_ASSERT_EQUAL(writer(expectedRootMesh), writer(mesh));
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, treeWithBranch_bones)
{
   MaterialDefinition matStub;

   // prepare the tree structure
   TreeSegment structure;
   structure.direction = D3DXVECTOR3(0, 1, 0);
   structure.position = D3DXVECTOR3(0, 0, 0);
   structure.width = 10;
   structure.type = BRANCH_START;
   structure.segmentIdx = 0;
   TreeSegment* parentSeg = &structure;

   TreeSegment* child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 5;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parentSeg->children.push_back(child);
   parentSeg = child;

   {
      float angle = D3DXToRadian(45); // 45 deg around OZ axis
      float c45 = cos(angle);
      float s45 = sin(angle);

      TreeSegment* branchRoot = new TreeSegment();
      branchRoot->rightVec = D3DXVECTOR3(c45, s45, 0);
      branchRoot->direction = D3DXVECTOR3(-s45, c45, 0);
      branchRoot->position = D3DXVECTOR3(0, 10, 0);
      branchRoot->width = 10;
      branchRoot->type = BRANCH_START;
      branchRoot->segmentIdx = 1;
      parentSeg->children.push_back(branchRoot);
      TreeSegment* branchParent = branchRoot;

      TreeSegment* branchChild = new TreeSegment();
      branchChild->rightVec = D3DXVECTOR3(c45, s45, 0);
      branchChild->direction = D3DXVECTOR3(-s45, c45, 0);
      branchChild->position = D3DXVECTOR3(-7.0710669f, 17.071068f, 0);
      branchChild->width = 5;
      branchChild->type = BRANCH_MIDDLE;
      branchChild->segmentIdx = 2;
      branchParent->children.push_back(branchChild);
      branchParent = branchChild;

      branchChild = new TreeSegment();
      branchChild->rightVec = D3DXVECTOR3(c45, s45, 0);
      branchChild->direction = D3DXVECTOR3(-s45, c45, 0);
      branchChild->position = D3DXVECTOR3(-14.142134f, 24.142136f, 0);
      branchChild->width = 0;
      branchChild->type = BRANCH_END;
      branchChild->segmentIdx = 3;
      branchParent->children.push_back(branchChild);
   }

   child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 2;
   parentSeg->children.push_back(child);


   // create the skin for the tree
   TreeSkinner skinner(structure);
   MeshDefinition mesh;
   unsigned int circRes = 4;
   unsigned int boneRes = 1;
   skinner("tree", circRes, boneRes, matStub, mesh);
   
   // prepare the results
   // ... the root
   MeshDefinition expectedRootMesh;
   expectedRootMesh.isSkin = true;
   expectedRootMesh.name = "tree";
   D3DXMatrixIdentity(&expectedRootMesh.localMtx);

   BonesInfluenceDefinition bonesForAttrib; 
   bonesForAttrib.push_back("treeBone_0");
   bonesForAttrib.push_back("treeBone_1");
   expectedRootMesh.bonesInfluencingAttribute.push_back(bonesForAttrib);

   D3DXMATRIX treeBone_0_OffsetMtx; 
   D3DXMatrixIdentity(&treeBone_0_OffsetMtx);
   D3DXMATRIX treeBone_1_OffsetMtx; 
   D3DXMatrixIdentity(&treeBone_1_OffsetMtx);
   treeBone_1_OffsetMtx._42 = -10;
   expectedRootMesh.skinBones.push_back(SkinBoneDefinition("treeBone_0", treeBone_0_OffsetMtx));
   expectedRootMesh.skinBones.push_back(SkinBoneDefinition("treeBone_1", treeBone_1_OffsetMtx));
  
   MeshDefinition* treeBone_0_Mesh = new MeshDefinition();
   treeBone_0_Mesh->name = "treeBone_0";
   treeBone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBone_0_Mesh->localMtx));
   expectedRootMesh.children.push_back(treeBone_0_Mesh);

   MeshDefinition* treeBone_1_Mesh = new MeshDefinition();
   treeBone_1_Mesh->name = "treeBone_1";
   treeBone_1_Mesh->isSkin = false;

   D3DXMATRIX mtxCombined = treeBone_0_Mesh->localMtx;
   D3DXMatrixIdentity(&(treeBone_1_Mesh->localMtx));
   treeBone_1_Mesh->localMtx._41 = 0; treeBone_1_Mesh->localMtx._42 = 10; treeBone_1_Mesh->localMtx._43 = 0;

   treeBone_0_Mesh->children.push_back(treeBone_1_Mesh);

   // ... the branch
   MeshDefinition* expectedBranchMesh = new MeshDefinition();
   expectedBranchMesh->isSkin = true;
   expectedBranchMesh->name = "tree_branch_0";
   
   D3DXMATRIX rotMtx;
   D3DXVECTOR3 zVec(0, 0, 1);
   D3DXMatrixRotationAxis(&rotMtx, &zVec, D3DXToRadian(45));
   D3DXMatrixTranslation(&(expectedBranchMesh->localMtx), 0, 10, 0);
   D3DXMatrixMultiply(&(expectedBranchMesh->localMtx), &rotMtx, &(expectedBranchMesh->localMtx));
   expectedRootMesh.children.push_back(expectedBranchMesh);


   bonesForAttrib.clear(); 
   bonesForAttrib.push_back("tree_branch_0Bone_0");
   bonesForAttrib.push_back("tree_branch_0Bone_1");
   expectedBranchMesh->bonesInfluencingAttribute.push_back(bonesForAttrib);

   D3DXMATRIX tree_branch_0Bone_0_OffsetMtx; 
   D3DXMatrixIdentity(&tree_branch_0Bone_0_OffsetMtx);
   D3DXMATRIX tree_branch_0Bone_1_OffsetMtx; 
   D3DXMatrixIdentity(&tree_branch_0Bone_1_OffsetMtx);
   tree_branch_0Bone_1_OffsetMtx._42 = -10;
   expectedBranchMesh->skinBones.push_back(SkinBoneDefinition("tree_branch_0Bone_0", tree_branch_0Bone_0_OffsetMtx));
   expectedBranchMesh->skinBones.push_back(SkinBoneDefinition("tree_branch_0Bone_1", tree_branch_0Bone_1_OffsetMtx));

   MeshDefinition* treeBranch_0_Bone_0_Mesh = new MeshDefinition();
   treeBranch_0_Bone_0_Mesh->name = "tree_branch_0Bone_0";
   treeBranch_0_Bone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBranch_0_Bone_0_Mesh->localMtx));
   expectedBranchMesh->children.push_back(treeBranch_0_Bone_0_Mesh);

   MeshDefinition* treeBranch_0_Bone_1_Mesh = new MeshDefinition();
   treeBranch_0_Bone_1_Mesh->name = "tree_branch_0Bone_1";
   treeBranch_0_Bone_1_Mesh->isSkin = false;
   D3DXMatrixTranslation(&(treeBranch_0_Bone_1_Mesh->localMtx), 0, 10, 0);
   treeBranch_0_Bone_0_Mesh->children.push_back(treeBranch_0_Bone_1_Mesh);

   MeshDefinitionWriter writer;
   writer.writeBasics(); CPPUNIT_ASSERT_EQUAL(writer(expectedRootMesh), writer(mesh));
   writer.writeBones(); CPPUNIT_ASSERT_EQUAL(writer(expectedRootMesh), writer(mesh));
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, treeWithTwoLevelsOfBRanches_relativeMatrices)
{
   MaterialDefinition matStub;

   // prepare the tree structure
   TreeSegment structure;
   structure.direction = D3DXVECTOR3(0, 1, 0);
   structure.position = D3DXVECTOR3(0, 0, 0);
   structure.width = 10;
   structure.type = BRANCH_START;
   structure.segmentIdx = 0;
   TreeSegment* parentSeg = &structure;

   TreeSegment* child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 6.66667f;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parentSeg->children.push_back(child);
   parentSeg = child;

   {
      TreeSegment* branchRoot = new TreeSegment();
      branchRoot->rightVec = D3DXVECTOR3(0.880236f, 0, 0.474536f);
      branchRoot->direction = D3DXVECTOR3(0.474536f, 0, -0.880236f);
      branchRoot->position = D3DXVECTOR3(0, 10, 0);
      branchRoot->width = 3.33333f;
      branchRoot->type = BRANCH_START;
      branchRoot->segmentIdx = 1;
      parentSeg->children.push_back(branchRoot);
      TreeSegment* branchParent = branchRoot;

      TreeSegment* branchChild = new TreeSegment();
      branchChild->rightVec = D3DXVECTOR3(0.880236f, 0, 0.474536f);
      branchChild->direction = D3DXVECTOR3(0.474536f, 0, -0.88023f);
      branchChild->position = D3DXVECTOR3(4.74536f, 10, -8.80236f);
      branchChild->width = 1.11111f;
      branchChild->type = BRANCH_MIDDLE;
      branchChild->segmentIdx = 2;
      branchParent->children.push_back(branchChild);
      branchParent = branchChild;

      {
         TreeSegment* subBranchRoot = new TreeSegment();
         subBranchRoot->rightVec = D3DXVECTOR3(0.804819f, -0.40499f, 0.433878f);
         subBranchRoot->direction = D3DXVECTOR3(0.356487f, 0.914321f, 0.192182f);
         subBranchRoot->position = D3DXVECTOR3(4.74536f, 10, -8.80236f);
         subBranchRoot->width = 0.555555f;
         subBranchRoot->type = BRANCH_START;
         subBranchRoot->segmentIdx = 2;
         branchParent->children.push_back(subBranchRoot);

         TreeSegment* subBranchChild = new TreeSegment();
         subBranchChild->rightVec = D3DXVECTOR3(0.804819f, -0.40499f, 0.433878f);
         subBranchChild->direction = D3DXVECTOR3(0.356487f, 0.914321f, 0.192182f);
         subBranchChild->position = D3DXVECTOR3(8.31022f, 19.1432f, -6.88054f);
         subBranchChild->width = 0;
         subBranchChild->type = BRANCH_END;
         subBranchChild->segmentIdx = 3;
         subBranchRoot->children.push_back(subBranchChild);
      }

      branchChild = new TreeSegment();
      branchChild->rightVec = D3DXVECTOR3(0.880236f, 0, 0.474536f);
      branchChild->direction = D3DXVECTOR3(0.474536f, 0, -0.880236f);
      branchChild->position = D3DXVECTOR3(9.49071f, 10, -17.6047f);
      branchChild->width = 0;
      branchChild->type = BRANCH_END;
      branchChild->segmentIdx = 3;
      branchParent->children.push_back(branchChild);
   }

   child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 3.33333f;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 2;
   parentSeg->children.push_back(child);
   parentSeg = child;

   {
      TreeSegment* branchRoot = new TreeSegment();
      branchRoot->rightVec = D3DXVECTOR3(-0.31783f, 0, -0.948148f);
      branchRoot->direction = D3DXVECTOR3(0.948148f,0, -0.31783f);
      branchRoot->position = D3DXVECTOR3(0, 20, 0);
      branchRoot->width = 1.66667f;
      branchRoot->type = BRANCH_START;
      branchRoot->segmentIdx = 2;
      parentSeg->children.push_back(branchRoot);

      TreeSegment* branchChild = new TreeSegment();
      branchChild->rightVec = D3DXVECTOR3(-0.31783f, 0, -0.948148f);
      branchChild->direction = D3DXVECTOR3(0.948148f, 0, -0.31783f);
      branchChild->position = D3DXVECTOR3(9.48148f, 20, -3.1783f);
      branchChild->width = 0;
      branchChild->type = BRANCH_END;
      branchChild->segmentIdx = 3;
      branchRoot->children.push_back(branchChild);
   }

   child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 30, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 3;
   parentSeg->children.push_back(child);


   // create the skin for the tree
   TreeSkinner skinner(structure);
   MeshDefinition mesh;
   unsigned int circRes = 4;
   unsigned int boneRes = 10;
   skinner("tree", circRes, boneRes, matStub, mesh);
   

   // prepare the results
   // ... the root
   MeshDefinition expectedRootMesh;
   expectedRootMesh.isSkin = true;
   expectedRootMesh.name = "tree";
   D3DXMatrixIdentity(&expectedRootMesh.localMtx);


   MeshDefinition* treeBone_0_Mesh = new MeshDefinition();
   treeBone_0_Mesh->name = "treeBone_0";
   treeBone_0_Mesh->isSkin = false;
   D3DXMatrixIdentity(&(treeBone_0_Mesh->localMtx));
   expectedRootMesh.children.push_back(treeBone_0_Mesh);


   // ... branch 0
   {
      MeshDefinition* tree_branch_0 = new MeshDefinition();
      tree_branch_0->isSkin = true;
      tree_branch_0->name = "tree_branch_0";
      tree_branch_0->localMtx._11 = 0.880236f;
      tree_branch_0->localMtx._12 = 0;
      tree_branch_0->localMtx._13 = 0.474536f;
      tree_branch_0->localMtx._14 = 0;
      tree_branch_0->localMtx._21 = 0.474536f;
      tree_branch_0->localMtx._22 = 0;
      tree_branch_0->localMtx._23 = -0.880236f;
      tree_branch_0->localMtx._24 = 0;
      tree_branch_0->localMtx._31 = 0;
      tree_branch_0->localMtx._32 = 1;
      tree_branch_0->localMtx._33 = 0;
      tree_branch_0->localMtx._34 = 0;
      tree_branch_0->localMtx._41 = 0;
      tree_branch_0->localMtx._42 = 10;
      tree_branch_0->localMtx._43 = 0;
      tree_branch_0->localMtx._44 = 1;
      expectedRootMesh.children.push_back(tree_branch_0);

      MeshDefinition* tree_branch_0Bone_0 = new MeshDefinition();
      tree_branch_0Bone_0->name = "tree_branch_0Bone_0";
      tree_branch_0Bone_0->isSkin = false;
      D3DXMatrixIdentity(&(tree_branch_0Bone_0->localMtx));
      tree_branch_0->children.push_back(tree_branch_0Bone_0);

      // ... branch 2
      {
         MeshDefinition* tree_branch_2 = new MeshDefinition();
         tree_branch_2->isSkin = true;
         tree_branch_2->name = "tree_branch_2";
         tree_branch_2->localMtx._11 = 0.914322f;
         tree_branch_2->localMtx._12 = 0;
         tree_branch_2->localMtx._13 = -0.40499f;
         tree_branch_2->localMtx._14 = 0;
         tree_branch_2->localMtx._21 = 0.40499f;
         tree_branch_2->localMtx._22 = 0;
         tree_branch_2->localMtx._23 = 0.914321f;
         tree_branch_2->localMtx._24 = 0;
         tree_branch_2->localMtx._31 = 0;
         tree_branch_2->localMtx._32 = -1;
         tree_branch_2->localMtx._33 = 0;
         tree_branch_2->localMtx._34 = 0;
         tree_branch_2->localMtx._41 = 0;
         tree_branch_2->localMtx._42 = 10;
         tree_branch_2->localMtx._43 = 0;
         tree_branch_2->localMtx._44 = 1;
         tree_branch_0->children.push_back(tree_branch_2);

         MeshDefinition* tree_branch_2Bone_0 = new MeshDefinition();
         tree_branch_2Bone_0->name = "tree_branch_2Bone_0";
         tree_branch_2Bone_0->isSkin = false;
         D3DXMatrixIdentity(&(tree_branch_2Bone_0->localMtx));
         tree_branch_2->children.push_back(tree_branch_2Bone_0);
      }
   }

   // ... branch 1
   {
      MeshDefinition* tree_branch_1 = new MeshDefinition();
      tree_branch_1->isSkin = true;
      tree_branch_1->name = "tree_branch_1";
      tree_branch_1->localMtx._11 = -0.31783f;
      tree_branch_1->localMtx._12 = 0;
      tree_branch_1->localMtx._13 = -0.948148f;
      tree_branch_1->localMtx._14 = 0;
      tree_branch_1->localMtx._21 = 0.948148f;
      tree_branch_1->localMtx._22 = 0;
      tree_branch_1->localMtx._23 = -0.31783f;
      tree_branch_1->localMtx._24 = 0;
      tree_branch_1->localMtx._31 = 0;
      tree_branch_1->localMtx._32 = -1;
      tree_branch_1->localMtx._33 = 0;
      tree_branch_1->localMtx._34 = 0;
      tree_branch_1->localMtx._41 = 0;
      tree_branch_1->localMtx._42 = 20;
      tree_branch_1->localMtx._43 = 0;
      tree_branch_1->localMtx._44 = 1;
      expectedRootMesh.children.push_back(tree_branch_1);

      MeshDefinition* tree_branch_1Bone_0 = new MeshDefinition();
      tree_branch_1Bone_0->name = "tree_branch_1Bone_0";
      tree_branch_1Bone_0->isSkin = false;
      D3DXMatrixIdentity(&(tree_branch_1Bone_0->localMtx));
      tree_branch_1->children.push_back(tree_branch_1Bone_0);
   }


   MeshDefinitionWriter writer;
   writer.writeBasics(); CPPUNIT_ASSERT_EQUAL(writer(expectedRootMesh), writer(mesh));
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, treeWithTwoLevelsOfBRanches_vertices)
{
   MaterialDefinition matStub;

   // prepare the tree structure
   TreeSegment structure;
   structure.direction = D3DXVECTOR3(0, 1, 0);
   structure.position = D3DXVECTOR3(0, 0, 0);
   structure.width = 10;
   structure.type = BRANCH_START;
   structure.segmentIdx = 0;
   TreeSegment* parentSeg = &structure;

   TreeSegment* child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 6.66667f;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parentSeg->children.push_back(child);
   parentSeg = child;

   {
      TreeSegment* branchRoot = new TreeSegment();
      branchRoot->rightVec = D3DXVECTOR3(-1, 0, 0);
      branchRoot->direction = D3DXVECTOR3(0, 0, 1);
      branchRoot->position = D3DXVECTOR3(0, 10, 0);
      branchRoot->width = 3.33333f;
      branchRoot->type = BRANCH_START;
      branchRoot->segmentIdx = 1;
      parentSeg->children.push_back(branchRoot);
      TreeSegment* branchParent = branchRoot;

      TreeSegment* branchChild = new TreeSegment();
      branchChild->rightVec = D3DXVECTOR3(-1, 0, 0);
      branchChild->direction = D3DXVECTOR3(0, 0, 1);
      branchChild->position = D3DXVECTOR3(0, 10, 10);
      branchChild->width = 1.11111f;
      branchChild->type = BRANCH_MIDDLE;
      branchChild->segmentIdx = 2;
      branchParent->children.push_back(branchChild);
      branchParent = branchChild;

      {
         TreeSegment* subBranchRoot = new TreeSegment();
         subBranchRoot->rightVec = D3DXVECTOR3(1, 0, 0);
         subBranchRoot->direction = D3DXVECTOR3(0, -1, 0);
         subBranchRoot->position = D3DXVECTOR3(0, 10, 10);
         subBranchRoot->width = 0.555555f;
         subBranchRoot->type = BRANCH_START;
         subBranchRoot->segmentIdx = 2;
         branchParent->children.push_back(subBranchRoot);

         TreeSegment* subBranchChild = new TreeSegment();
         subBranchChild->rightVec = D3DXVECTOR3(1, 0, 0);
         subBranchChild->direction = D3DXVECTOR3(0, -1, 0);
         subBranchChild->position = D3DXVECTOR3(0, 0, 10);
         subBranchChild->width = 0;
         subBranchChild->type = BRANCH_END;
         subBranchChild->segmentIdx = 3;
         subBranchRoot->children.push_back(subBranchChild);
      }

      branchChild = new TreeSegment();
      branchChild->rightVec = D3DXVECTOR3(-1, 0, 0);
      branchChild->direction = D3DXVECTOR3(0, 0, 1);
      branchChild->position = D3DXVECTOR3(0, 10, 20);
      branchChild->width = 0;
      branchChild->type = BRANCH_END;
      branchChild->segmentIdx = 3;
      branchParent->children.push_back(branchChild);
   }

   child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 3.33333f;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 2;
   parentSeg->children.push_back(child);
   parentSeg = child;

   {
      TreeSegment* branchRoot = new TreeSegment();
      branchRoot->rightVec = D3DXVECTOR3(-1, 0, 0);
      branchRoot->direction = D3DXVECTOR3(0, 0, -1);
      branchRoot->position = D3DXVECTOR3(0, 20, 0);
      branchRoot->width = 1.66667f;
      branchRoot->type = BRANCH_START;
      branchRoot->segmentIdx = 2;
      parentSeg->children.push_back(branchRoot);

      TreeSegment* branchChild = new TreeSegment();
      branchChild->rightVec = D3DXVECTOR3(-1, 0, 0);
      branchChild->direction = D3DXVECTOR3(0, 0, -1);
      branchChild->position = D3DXVECTOR3(0, 20, -10);
      branchChild->width = 0;
      branchChild->type = BRANCH_END;
      branchChild->segmentIdx = 3;
      branchRoot->children.push_back(branchChild);
   }

   child = new TreeSegment();
   child->direction = D3DXVECTOR3(0, 1, 0);
   child->position = D3DXVECTOR3(0, 30, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 3;
   parentSeg->children.push_back(child);


   // create the skin for the tree
   TreeSkinner skinner(structure);
   MeshDefinition mesh;
   unsigned int circRes = 3;
   unsigned int boneRes = 10;
   skinner("tree", circRes, boneRes, matStub, mesh);
   

   // prepare the results
   // ... the root
   MeshDefinition expectedRootMesh;
   expectedRootMesh.vertices.push_back(LitVertex(5, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0)); 
   expectedRootMesh.vertices.push_back(LitVertex(-2.5f, 0, 4.33013f, 1, 0, 0, -0.5f, 0, 0.866025f, 0.333333f, 0));
   expectedRootMesh.vertices.push_back(LitVertex(-2.5f, 0, -4.33013f, 1, 0, 0, -0.5f, 0, -0.866025f, 0.666667f, 0));
   expectedRootMesh.vertices.push_back(LitVertex(5, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0));
   expectedRootMesh.vertices.push_back(LitVertex(3.33333f, 10, 0, 1, 0, 0, 1, 0, 0, 0, 0.333333f));
   expectedRootMesh.vertices.push_back(LitVertex(-1.66667f, 10, 2.88675f, 1, 0, 0, -0.5f, 0, 0.866025f, 0.333333f, 0.333333f));
   expectedRootMesh.vertices.push_back(LitVertex(-1.66667f, 10, -2.88675f, 1, 0, 0, -0.5, 0, -0.866025f, 0.666667f, 0.333333f));
   expectedRootMesh.vertices.push_back(LitVertex(3.33333f, 10, 0, 1, 0, 0, 1, 0, 0, 1, 0.333333f));
   expectedRootMesh.vertices.push_back(LitVertex(1.66666f, 20, 0, 1, 0, 0, 1, 0, 0, 0, 0.666667f));
   expectedRootMesh.vertices.push_back(LitVertex(-0.833333f, 20, 1.44337f, 1, 0, 0, -0.5f, 0, 0.866025f, 0.333333f, 0.666667f));
   expectedRootMesh.vertices.push_back(LitVertex(-0.833332f, 20, -1.44337f, 1, 0, 0, -0.5f, 0, -0.866025f, 0.666667f, 0.666667f));
   expectedRootMesh.vertices.push_back(LitVertex(1.66666f, 20, 0, 1, 0, 0, 1, 0, 0, 1, 0.666667f));
   expectedRootMesh.vertices.push_back(LitVertex(0, 30, 0, 1, 0, 0, 0, 1, 0, 0.5f, 1));

   MeshDefinition* treeBone_0_Mesh = new MeshDefinition();
   expectedRootMesh.children.push_back(treeBone_0_Mesh);


   // ... branch 0
   {
      MeshDefinition* tree_branch_0 = new MeshDefinition();
      tree_branch_0->vertices.push_back(LitVertex(1.66666f, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0.333333f));
      tree_branch_0->vertices.push_back(LitVertex(-0.833333f, 0, 1.44337f, 1, 0, 0, -0.5f, 0, 0.866025f, 0.333333f, 0.333333f));
      tree_branch_0->vertices.push_back(LitVertex(-0.833332f, 0, -1.44337f, 1, 0, 0, -0.5f, 0, -0.866025f, 0.666667f, 0.333333f));
      tree_branch_0->vertices.push_back(LitVertex(1.66666f, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0.333333f));
      tree_branch_0->vertices.push_back(LitVertex(0.555555f, 10, 0, 1, 0, 0, 1, 0, 0, 0, 0.666667f));
      tree_branch_0->vertices.push_back(LitVertex(-0.277777f, 10, 0.481125f, 1, 0, 0, -0.5f, 0, 0.866025f, 0.333333f, 0.666667f));
      tree_branch_0->vertices.push_back(LitVertex(-0.277777f, 10, -0.481125f, 1, 0, 0, -0.5f, 0, -0.866025f, 0.666667f, 0.666667f));
      tree_branch_0->vertices.push_back(LitVertex(0.555555f, 10, 0, 1, 0, 0, 1, 0, 0, 1, 0.666667f));
      tree_branch_0->vertices.push_back(LitVertex(0, 20, 0, 1, 0, 0, 0, 1, 0, 0.5f, 1));
      expectedRootMesh.children.push_back(tree_branch_0);

      MeshDefinition* tree_branch_0Bone_0 = new MeshDefinition();
      tree_branch_0->children.push_back(tree_branch_0Bone_0);

      // ... branch 2
      {
         MeshDefinition* tree_branch_2 = new MeshDefinition();
         tree_branch_2->vertices.push_back(LitVertex(0.277777f, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0.666667f)); 
         tree_branch_2->vertices.push_back(LitVertex(-0.138889f, 0, 0.240562f, 1, 0, 0, -0.5f, 0, 0.866025f, 0.333333f, 0.666667f));
         tree_branch_2->vertices.push_back(LitVertex(-0.138889f, 0, -0.240562f, 1, 0, 0, -0.5f, 0, -0.866025f, 0.666667f, 0.666667f));
         tree_branch_2->vertices.push_back(LitVertex(0.277777f, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0.666667f));
         tree_branch_2->vertices.push_back(LitVertex(0, 10, 0, 1, 0, 0, 0, 1, 0, 0.5f, 1));
         tree_branch_0->children.push_back(tree_branch_2);

         MeshDefinition* tree_branch_2Bone_0 = new MeshDefinition();
         tree_branch_2->children.push_back(tree_branch_2Bone_0);
      }
   }

   // ... branch 1
   {
      MeshDefinition* tree_branch_1 = new MeshDefinition();
      tree_branch_1->vertices.push_back(LitVertex(0.833335f, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0.666667f));
      tree_branch_1->vertices.push_back(LitVertex(-0.416668f, 0, 0.721689f, 1, 0, 0, -0.5f, 0, 0.866025f, 0.333333f, 0.666667f));
      tree_branch_1->vertices.push_back(LitVertex(-0.416667f, 0, -0.721689f, 1, 0, 0, -0.5f, 0, -0.866025f, 0.666667f, 0.666667f));
      tree_branch_1->vertices.push_back(LitVertex(0.833335f, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0.666667f));
      tree_branch_1->vertices.push_back(LitVertex(0, 10, 0, 1, 0, 0, 0, 1, 0, 0.5f, 1));
      expectedRootMesh.children.push_back(tree_branch_1);

      MeshDefinition* tree_branch_1Bone_0 = new MeshDefinition();
      tree_branch_1->children.push_back(tree_branch_1Bone_0);
   }


   MeshDefinitionWriter writer;
   writer.writeVertices(); CPPUNIT_ASSERT_EQUAL(writer(expectedRootMesh), writer(mesh));
}

//////////////////////////////////////////////////////////////////////////////
