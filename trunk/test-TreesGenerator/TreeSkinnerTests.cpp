#include "core-TestFramework\TestFramework.h"
#include "ext-TreesGenerator\TreeSegment.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include "MeshDefinitionWriter.h"
#include "ext-TreesGenerator\TreeSkinner.h"
#include "SampleTrees.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, bonesStructure)
{
   MaterialDefinition matStub("matStub");

   // prepare the tree structure
   TreeSegment structure;
   SampleTrees::threeBranchTree(structure);

   // create the skin for the tree
   TreeSkinner skinner(structure);
   unsigned int circRes = 3;
   unsigned int boneRes = 1;
   MeshDefinition* mesh = skinner("tree", circRes, boneRes, matStub);

   // prepare the results

   // root branch
   MeshDefinition branch0Bone0;
   branch0Bone0.isSkin = false;
   branch0Bone0.name = "branch_0_Bone_0";
   D3DXMatrixIdentity(&branch0Bone0.localMtx);

   MeshDefinition* branch0Skin = new MeshDefinition();
   branch0Skin->name = "branch_0_skin";
   branch0Skin->isSkin = true;
   D3DXMatrixIdentity(&(branch0Skin->localMtx));
   branch0Bone0.children.push_back(branch0Skin);

   MeshDefinition* branch0Bone1 = new MeshDefinition();
   branch0Bone1->name = "branch_0_Bone_1";
   branch0Bone1->isSkin = false;
   D3DXMatrixTranslation(&(branch0Bone1->localMtx), 0, 10, 0);
   branch0Bone0.children.push_back(branch0Bone1);

       // first root branch (one that branches off the branch_0_Bone_1)
      MeshDefinition* branch1Bone0 = new MeshDefinition();
      branch1Bone0->name = "branch_1_Bone_0";
      branch1Bone0->isSkin = false;
      D3DXMatrixIdentity(&branch1Bone0->localMtx);
      branch1Bone0->localMtx._22 = 0; branch1Bone0->localMtx._23 = 1;
      branch1Bone0->localMtx._32 = -1; branch1Bone0->localMtx._33 = 0;
      branch0Bone1->children.push_back(branch1Bone0);

      MeshDefinition* branch1Skin = new MeshDefinition();
      branch1Skin->name = "branch_1_skin";
      branch1Skin->isSkin = true;
      D3DXMatrixIdentity(&(branch1Skin->localMtx));
      branch1Bone0->children.push_back(branch1Skin);

      MeshDefinition* branch1Bone1 = new MeshDefinition();
      branch1Bone1->name = "branch_1_Bone_1";
      branch1Bone1->isSkin = false;
      D3DXMatrixTranslation(&(branch1Bone1->localMtx), 0, 10, 0);
      branch1Bone0->children.push_back(branch1Bone1);

         // branch 1 branch (one that branches off the branch_1_Bone_1)
         MeshDefinition* branch3Bone0 = new MeshDefinition();
         branch3Bone0->name = "branch_3_Bone_0";
         branch3Bone0->isSkin = false;
         D3DXMatrixIdentity(&branch3Bone0->localMtx);
         branch3Bone0->localMtx._22 = 0; branch3Bone0->localMtx._23 = 1;
         branch3Bone0->localMtx._32 = -1; branch3Bone0->localMtx._33 = 0;
         branch1Bone1->children.push_back(branch3Bone0);

         MeshDefinition* branch3Skin = new MeshDefinition();
         branch3Skin->name = "branch_3_skin";
         branch3Skin->isSkin = true;
         D3DXMatrixIdentity(&(branch3Skin->localMtx));
         branch3Bone0->children.push_back(branch3Skin);

   MeshDefinition* branch0Bone2 = new MeshDefinition();
   branch0Bone2->name = "branch_0_Bone_2";
   branch0Bone2->isSkin = false;
   D3DXMatrixTranslation(&(branch0Bone2->localMtx), 0, 10, 0);
   branch0Bone1->children.push_back(branch0Bone2);

      // second root branch (one that branches off the branch_0_Bone_2)
      MeshDefinition* branch2Bone0 = new MeshDefinition();
      branch2Bone0->name = "branch_2_Bone_0";
      branch2Bone0->isSkin = false;
      D3DXMatrixIdentity(&branch2Bone0->localMtx);
      branch2Bone0->localMtx._11 = -1;
      branch2Bone0->localMtx._22 = 0; branch2Bone0->localMtx._23 = -1;
      branch2Bone0->localMtx._32 = -1; branch2Bone0->localMtx._33 = 0;
      branch0Bone2->children.push_back(branch2Bone0);

      MeshDefinition* branch2Skin = new MeshDefinition();
      branch2Skin->name = "branch_2_skin";
      branch2Skin->isSkin = true;
      D3DXMatrixIdentity(&(branch2Skin->localMtx));
      branch2Bone0->children.push_back(branch2Skin);


   // run the comparisons
   MeshDefinitionWriter writer;
   writer.writeBasics(); CPPUNIT_ASSERT_EQUAL(writer(branch0Bone0), writer(*mesh));
   delete mesh;
}

///////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, sparseBoneStructure)
{
   MaterialDefinition matStub("matStub");

   // prepare the tree structure
   TreeSegment structure;
   SampleTrees::highTwoBranchTree(structure);

   // create the skin for the tree
   TreeSkinner skinner(structure);
   unsigned int circRes = 3;
   unsigned int boneRes = 10;
   MeshDefinition* mesh = skinner("tree", circRes, boneRes, matStub);

   // prepare the results
   MeshDefinition branch0Bone0;
   branch0Bone0.isSkin = false;
   branch0Bone0.name = "branch_0_Bone_0";
   D3DXMatrixIdentity(&branch0Bone0.localMtx);

   MeshDefinition* branch0Skin = new MeshDefinition();
   branch0Skin->name = "branch_0_skin";
   branch0Skin->isSkin = true;
   D3DXMatrixIdentity(&(branch0Skin->localMtx));
   branch0Bone0.children.push_back(branch0Skin);

   MeshDefinition* branch0Bone1 = new MeshDefinition();
   branch0Bone1->name = "branch_0_Bone_1";
   branch0Bone1->isSkin = false;
   D3DXMatrixTranslation(&(branch0Bone1->localMtx), 0, 20, 0);
   branch0Bone0.children.push_back(branch0Bone1);

      // a branch
      MeshDefinition* branch1Bone0 = new MeshDefinition();
      branch1Bone0->name = "branch_1_Bone_0";
      branch1Bone0->isSkin = false;
      D3DXMatrixIdentity(&branch1Bone0->localMtx);
      branch1Bone0->localMtx._11 = -1;
      branch1Bone0->localMtx._22 = 0; branch1Bone0->localMtx._23 = -1;
      branch1Bone0->localMtx._32 = -1; branch1Bone0->localMtx._33 = 0;
      branch0Bone1->children.push_back(branch1Bone0);

      MeshDefinition* branch1Skin = new MeshDefinition();
      branch1Skin->name = "branch_1_skin";
      branch1Skin->isSkin = true;
      D3DXMatrixIdentity(&(branch1Skin->localMtx));
      branch1Bone0->children.push_back(branch1Skin);


   // run the comparisons
   MeshDefinitionWriter writer;
   writer.writeBasics(); CPPUNIT_ASSERT_EQUAL(writer(branch0Bone0), writer(*mesh));

   delete mesh;
}

///////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, verticesOfMesh)
{
   MaterialDefinition matStub("matStub");

   // prepare the tree structure
   TreeSegment structure;
   SampleTrees::threeBranchTree(structure);


   // create the skin for the tree
   TreeSkinner skinner(structure);
   unsigned int circRes = 4;
   unsigned int boneRes = 10;
   MeshDefinition* mesh = skinner("tree", circRes, boneRes, matStub);
   

   // prepare the results
   MeshDefinition branch0Bone0;
   MeshDefinition* branch0Skin = new MeshDefinition();
   branch0Bone0.children.push_back(branch0Skin);

   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,   1, 0, 0,    1, 0, 0,   0, 0)); 
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0, 5,   1, 0, 0,    0, 0, 1,   0.25f, 0));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 0, 0,   1, 0, 0,   -1, 0, 0,   0.5f, 0));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0,-5,   1, 0, 0,    0, 0,-1,   0.75f, 0));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,   1, 0, 0,    1, 0, 0,   1, 0));

   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 10, 0,   0, 1, 0,    1, 0, 0,   0, 0.333333f)); 
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 10, 5,   0, 1, 0,    0, 0, 1,   0.25f, 0.333333f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 10, 0,   0, 1, 0,   -1, 0, 0,   0.5f, 0.333333f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 10,-5,   0, 1, 0,    0, 0,-1,   0.75f, 0.333333f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 10, 0,   0, 1, 0,    1, 0, 0,   1, 0.333333f));

   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 20, 0,   1, 0, 0,    1, 0, 0,   0, 0.666667f)); 
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 20, 5,   1, 0, 0,    0, 0, 1,   0.25f, 0.666667f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 20, 0,   1, 0, 0,   -1, 0, 0,   0.5f, 0.666667f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 20,-5,   1, 0, 0,    0, 0,-1,   0.75f, 0.666667f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 20, 0,   1, 0, 0,    1, 0, 0,   1, 0.666667f));

   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 30, 0,   0, 1, 0,          0, 1, 0,   0.5f, 1));


   MeshDefinition* branch0Bone1 = new MeshDefinition();
   branch0Bone0.children.push_back(branch0Bone1);


       // first root branch (one that branches off the branch_0_Bone_1)
      MeshDefinition* branch1Bone0 = new MeshDefinition();
      branch0Bone1->children.push_back(branch1Bone0);

      MeshDefinition* branch1Skin = new MeshDefinition();
      branch1Bone0->children.push_back(branch1Skin);

      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,   1, 0, 0,    1, 0, 0,   0, 0.333333f)); 
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0, 5,   1, 0, 0,    0, 0, 1,   0.25f, 0.333333f));
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 0, 0,   1, 0, 0,   -1, 0, 0,   0.5f, 0.333333f));
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0,-5,   1, 0, 0,    0, 0,-1,   0.75f, 0.333333f));
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,   1, 0, 0,    1, 0, 0,   1, 0.333333f));

      // 2nd seg - where the bone is
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 10, 0,   0, 1, 0,    1, 0, 0,   0, 0.666667f)); 
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 10, 5,   0, 1, 0,    0, 0, 1,   0.25f, 0.666667f));
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 10, 0,   0, 1, 0,   -1, 0, 0,   0.5f, 0.666667f));
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 10,-5,   0, 1, 0,    0, 0,-1,   0.75f, 0.666667f));
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 10, 0,   0, 1, 0,    1, 0, 0,   1, 0.666667f));

      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 20, 0,   1, 0, 0,          0, 1, 0,   0.5f, 1));


      MeshDefinition* branch1Bone1 = new MeshDefinition();
      branch1Bone0->children.push_back(branch1Bone1);

         // branch 1 branch (one that branches off the branch_1_Bone_1)
         MeshDefinition* branch3Bone0 = new MeshDefinition();
         branch1Bone1->children.push_back(branch3Bone0);

         MeshDefinition* branch3Skin = new MeshDefinition();
         branch3Bone0->children.push_back(branch3Skin);

         branch3Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,  1, 0, 0,    1, 0, 0,   0, 0.666667f)); 
         branch3Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0, 5,  1, 0, 0,    0, 0, 1,   0.25f, 0.666667f));
         branch3Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 0, 0,  1, 0, 0,   -1, 0, 0,   0.5f, 0.666667f));
         branch3Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0,-5,  1, 0, 0,    0, 0,-1,   0.75f, 0.666667f));
         branch3Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,  1, 0, 0,    1, 0, 0,   1, 0.666667f));

         branch3Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 10, 0,  1, 0, 0,   0, 1, 0,   0.5f, 1));

   MeshDefinition* branch0Bone2 = new MeshDefinition();
   branch0Bone1->children.push_back(branch0Bone2);

      // second root branch (one that branches off the branch_0_Bone_2)
      MeshDefinition* branch2Bone0 = new MeshDefinition();
      branch0Bone2->children.push_back(branch2Bone0);

      MeshDefinition* branch2Skin = new MeshDefinition();
      branch2Bone0->children.push_back(branch2Skin);

      branch2Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,  1, 0, 0,    1, 0, 0,   0, 0.666667f)); 
      branch2Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0, 5,  1, 0, 0,    0, 0, 1,   0.25f, 0.666667f));
      branch2Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 0, 0,  1, 0, 0,   -1, 0, 0,   0.5f, 0.666667f));
      branch2Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0,-5,  1, 0, 0,    0, 0,-1,   0.75f, 0.666667f));
      branch2Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,  1, 0, 0,    1, 0, 0,   1, 0.666667f));

      branch2Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 10, 0,   1, 0, 0,  0, 1, 0,   0.5f, 1));


   MeshDefinitionWriter writer;
   writer.writeVertices(); CPPUNIT_ASSERT_EQUAL(writer(branch0Bone0), writer(*mesh));

   delete mesh;
}

///////////////////////////////////////////////////////////////////////////////

TEST(TreeSkinner, skinDetailsOfSparseMesh)
{
   MaterialDefinition matStub("matStub");

   // prepare the tree structure
   TreeSegment structure;
   SampleTrees::higherTwoBranchTree(structure);


   // create the skin for the tree
   TreeSkinner skinner(structure);
   unsigned int circRes = 4;
   unsigned int boneRes = 10;
   MeshDefinition* mesh = skinner("tree", circRes, boneRes, matStub);
   

   // prepare the results
   BonesInfluenceDefinition bonesForAttibute;
   D3DXMATRIX offsetMtx; 

   MeshDefinition branch0Bone0;
   branch0Bone0.name = "branch_0_Bone_0";

   MeshDefinition* branch0Skin = new MeshDefinition();
   branch0Skin->name = "branch_0_skin";
   branch0Bone0.children.push_back(branch0Skin);

   D3DXMatrixIdentity(&offsetMtx);
   branch0Skin->skinBones.push_back(SkinBoneDefinition("branch_0_Bone_0", offsetMtx));


   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,   1, 0, 0,    1, 0, 0,   0, 0)); 
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0, 5,   1, 0, 0,    0, 0, 1,   0.25f, 0));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 0, 0,   1, 0, 0,   -1, 0, 0,   0.5f, 0));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0,-5,   1, 0, 0,    0, 0,-1,   0.75f, 0));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,   1, 0, 0,    1, 0, 0,   1, 0));

   branch0Skin->faces.push_back(Face<USHORT> (0, 5, 1, 0));
   branch0Skin->faces.push_back(Face<USHORT> (1, 5, 6, 0));
   branch0Skin->faces.push_back(Face<USHORT> (1, 6, 2, 0));
   branch0Skin->faces.push_back(Face<USHORT> (2, 6, 7, 0));
   branch0Skin->faces.push_back(Face<USHORT> (2, 7, 3, 0));
   branch0Skin->faces.push_back(Face<USHORT> (3, 7, 8, 0));
   branch0Skin->faces.push_back(Face<USHORT> (3, 8, 4, 0));
   branch0Skin->faces.push_back(Face<USHORT> (4, 8, 9, 0));
   branch0Skin->faces.push_back(Face<USHORT> (4, 9, 0, 0));
   branch0Skin->faces.push_back(Face<USHORT> (0, 9, 5, 0));

   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 10, 0,   1, 0, 0,    1, 0, 0,   0, 0.25f)); 
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 10, 5,   1, 0, 0,    0, 0, 1,   0.25f, 0.25f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 10, 0,   1, 0, 0,   -1, 0, 0,   0.5f, 0.25f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 10,-5,   1, 0, 0,    0, 0,-1,   0.75f, 0.25f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 10, 0,   1, 0, 0,    1, 0, 0,   1, 0.25f));

   branch0Skin->faces.push_back(Face<USHORT> (5, 10,  6, 0));
   branch0Skin->faces.push_back(Face<USHORT> (6, 10, 11, 0));
   branch0Skin->faces.push_back(Face<USHORT> (6, 11,  7, 0));
   branch0Skin->faces.push_back(Face<USHORT> (7, 11, 12, 0));
   branch0Skin->faces.push_back(Face<USHORT> (7, 12,  8, 0));
   branch0Skin->faces.push_back(Face<USHORT> (8, 12, 13, 0));
   branch0Skin->faces.push_back(Face<USHORT> (8, 13,  9, 0));
   branch0Skin->faces.push_back(Face<USHORT> (9, 13, 14, 0));
   branch0Skin->faces.push_back(Face<USHORT> (9, 14,  5, 0));
   branch0Skin->faces.push_back(Face<USHORT> (5, 14, 10, 0));

   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 20, 0,   1, 0, 0,    1, 0, 0,   0, 0.5f)); 
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 20, 5,   1, 0, 0,    0, 0, 1,   0.25f, 0.5f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 20, 0,   1, 0, 0,   -1, 0, 0,   0.5f, 0.5f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 20,-5,   1, 0, 0,    0, 0,-1,   0.75f, 0.5f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 20, 0,   1, 0, 0,    1, 0, 0,   1, 0.5f));

   bonesForAttibute.clear(); 
   bonesForAttibute.push_back("branch_0_Bone_0"); 
   branch0Skin->bonesInfluencingAttribute.push_back(bonesForAttibute);

   branch0Skin->faces.push_back(Face<USHORT> (10, 15, 11, 1));
   branch0Skin->faces.push_back(Face<USHORT> (11, 15, 16, 1));
   branch0Skin->faces.push_back(Face<USHORT> (11, 16, 12, 1));
   branch0Skin->faces.push_back(Face<USHORT> (12, 16, 17, 1));
   branch0Skin->faces.push_back(Face<USHORT> (12, 17, 13, 1));
   branch0Skin->faces.push_back(Face<USHORT> (13, 17, 18, 1));
   branch0Skin->faces.push_back(Face<USHORT> (13, 18, 14, 1));
   branch0Skin->faces.push_back(Face<USHORT> (14, 18, 19, 1));
   branch0Skin->faces.push_back(Face<USHORT> (14, 19, 10, 1));
   branch0Skin->faces.push_back(Face<USHORT> (10, 19, 15, 1));

   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 30, 0,   0, 1, 0,    1, 0, 0,   0, 0.75f)); 
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 30, 5,   0, 1, 0,    0, 0, 1,   0.25f, 0.75f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 30, 0,   0, 1, 0,   -1, 0, 0,   0.5f, 0.75f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 30,-5,   0, 1, 0,    0, 0,-1,   0.75f, 0.75f));
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 30, 0,   0, 1, 0,    1, 0, 0,   1, 0.75f));

   bonesForAttibute.clear(); 
   bonesForAttibute.push_back("branch_0_Bone_0"); 
   bonesForAttibute.push_back("branch_0_Bone_1");
   branch0Skin->bonesInfluencingAttribute.push_back(bonesForAttibute);

   branch0Skin->faces.push_back(Face<USHORT> (15, 20, 16, 2));
   branch0Skin->faces.push_back(Face<USHORT> (16, 20, 17, 2));
   branch0Skin->faces.push_back(Face<USHORT> (17, 20, 18, 2));
   branch0Skin->faces.push_back(Face<USHORT> (18, 20, 19, 2));
   branch0Skin->faces.push_back(Face<USHORT> (19, 20, 15, 2));
   
   branch0Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 40, 0,   1, 0, 0,    0, 1, 0,   0.5f, 1));

   bonesForAttibute.clear(); 
   bonesForAttibute.push_back("branch_0_Bone_1");
   bonesForAttibute.push_back("branch_0_Bone_1");
   branch0Skin->bonesInfluencingAttribute.push_back(bonesForAttibute);

   // end of skin 0

   MeshDefinition* branch0Bone1 = new MeshDefinition();
   branch0Bone1->name = "branch_0_Bone_1";
   branch0Bone0.children.push_back(branch0Bone1);
   D3DXMatrixTranslation(&offsetMtx, 0, -30, 0);
   branch0Skin->skinBones.push_back(SkinBoneDefinition("branch_0_Bone_1", offsetMtx));

      // a branch
      MeshDefinition* branch1Bone0 = new MeshDefinition();
      branch1Bone0->name = "branch_1_Bone_0";
      branch0Bone1->children.push_back(branch1Bone0);

      MeshDefinition* branch1Skin = new MeshDefinition();
      branch1Skin->name = "branch_1_skin";
      branch1Bone0->children.push_back(branch1Skin);

      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,   1, 0, 0,    1, 0, 0,   0, 0.75f)); 
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0, 5,   1, 0, 0,    0, 0, 1,   0.25f, 0.75f));
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex(-5, 0, 0,   1, 0, 0,   -1, 0, 0,   0.5f, 0.75f));
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 0,-5,   1, 0, 0,    0, 0,-1,   0.75f, 0.75f));
      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 5, 0, 0,   1, 0, 0,    1, 0, 0,   1, 0.75f));

      branch1Skin->faces.push_back(Face<USHORT> (0, 5, 1, 0));
      branch1Skin->faces.push_back(Face<USHORT> (1, 5, 2, 0));
      branch1Skin->faces.push_back(Face<USHORT> (2, 5, 3, 0));
      branch1Skin->faces.push_back(Face<USHORT> (3, 5, 4, 0));
      branch1Skin->faces.push_back(Face<USHORT> (4, 5, 0, 0));

      branch1Skin->vertices.push_back(LitVertex::skinnedOneTex( 0, 10, 0,   1, 0, 0,          0, 1, 0,   0.5f, 1));

      bonesForAttibute.clear(); 
      bonesForAttibute.push_back("branch_1_Bone_0"); 
      branch1Skin->bonesInfluencingAttribute.push_back(bonesForAttibute);
      D3DXMatrixIdentity(&offsetMtx);
      branch1Skin->skinBones.push_back(SkinBoneDefinition("branch_1_Bone_0", offsetMtx));


   MeshDefinitionWriter writer;
   writer.writeVertices(); CPPUNIT_ASSERT_EQUAL(writer(branch0Bone0), writer(*mesh));
   writer.writeFaces(); CPPUNIT_ASSERT_EQUAL(writer(branch0Bone0), writer(*mesh));
   writer.writeBones(); CPPUNIT_ASSERT_EQUAL(writer(branch0Bone0), writer(*mesh));

   delete mesh;
}

///////////////////////////////////////////////////////////////////////////////
