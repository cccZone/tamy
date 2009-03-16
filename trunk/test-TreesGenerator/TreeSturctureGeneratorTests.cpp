#include "core-TestFramework\TestFramework.h"
#include "ext-TreesGenerator\TreeStructureGenerator.h"
#include "ext-TreesGenerator\TreeParams.h"
#include "ext-TreesGenerator\TreeSegment.h"
#include <d3dx9.h>
#include "TreeStructureWriter.h"
#include <deque>


//////////////////////////////////////////////////////////////////////////////

TEST(TreeStructureGenerator, creatingSingleSegmentTrunk)
{
   // generate the tree
   TreeParams params;
   params.maxTreeDepth = 1;
   params.startWidth = 40;
   params.segmentInitialLength = 10;
   params.startDirection = D3DXVECTOR3(0, 1, 0);

   TreeStructureGenerator gen;
   TreeSegment* treeSkel = gen.generate(params);
   CPPUNIT_ASSERT(treeSkel != NULL);

   // prepare the expected results
   TreeSegment* expectedStructure = new TreeSegment();
   expectedStructure->direction = params.startDirection;
   expectedStructure->position = D3DXVECTOR3(0, 0, 0);
   expectedStructure->width = params.startWidth;
   expectedStructure->type = BRANCH_START;
   expectedStructure->segmentIdx = 0;

   TreeSegment* child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 1;

   expectedStructure->children.push_back(child);

   // run the test
   TreeStructureWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedStructure), writer(*treeSkel));

   delete expectedStructure;
   delete treeSkel;
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeStructureGenerator, creatingLongTrunkWithnoBranches)
{
   // generate the tree
   TreeParams params;
   params.maxTreeDepth = 3;
   params.startWidth = 30;
   params.segmentInitialLength = 10;
   params.startDirection = D3DXVECTOR3(0, 1, 0);

   TreeStructureGenerator gen;
   TreeSegment* treeSkel = gen.generate(params);
   CPPUNIT_ASSERT(treeSkel != NULL);

   // prepare the expected results
   TreeSegment* parent = NULL; 
   TreeSegment* expectedStructure = new TreeSegment();
   expectedStructure->direction = params.startDirection;
   expectedStructure->position = D3DXVECTOR3(0, 0, 0);
   expectedStructure->width = 30;
   expectedStructure->type = BRANCH_START;
   expectedStructure->segmentIdx = 0;
   parent = expectedStructure;

   TreeSegment* child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 20;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parent->children.push_back(child);
   parent = child;

   child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 10;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 2;
   parent->children.push_back(child);
   parent = child;

   child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 30, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 3;
   parent->children.push_back(child);


   // run the test
   TreeStructureWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedStructure), writer(*treeSkel));

   delete expectedStructure;
   delete treeSkel;
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeStructureGenerator, branchDirectionVariation)
{
   // generate the tree
   TreeParams params;
   params.maxTreeDepth = 50;
   params.startWidth = 30;
   params.segmentInitialLength = 10;
   params.minInternalBranchDirVariation = 45;
   params.maxInternalBranchDirVariation = 60;
   params.startDirection = D3DXVECTOR3(0, 1, 0);

   TreeStructureGenerator gen;
   TreeSegment* treeSkel = gen.generate(params);
   CPPUNIT_ASSERT(treeSkel != NULL);
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int>(1), treeSkel->children.size());

   TreeSegment* parentSeg = treeSkel;
   TreeSegment* analyzedSeg = treeSkel->children.back();
   while(analyzedSeg != NULL)
   {
      CPPUNIT_ASSERT(parentSeg->direction != analyzedSeg->direction);

      float dirVariationAngle = D3DXVec3Dot(&(analyzedSeg->direction), &(parentSeg->direction));
      dirVariationAngle = D3DXToDegree(acos(dirVariationAngle));
      CPPUNIT_ASSERT(dirVariationAngle >= params.minInternalBranchDirVariation);
      CPPUNIT_ASSERT(dirVariationAngle <= params.maxInternalBranchDirVariation);

      CPPUNIT_ASSERT(analyzedSeg->children.size() <= 1);
      if (analyzedSeg->children.size() == 1)
      {
         parentSeg = analyzedSeg;
         analyzedSeg = analyzedSeg->children.back();
      }
      else
      {
         analyzedSeg = NULL;
      }
   }

   delete treeSkel;
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeStructureGenerator, treeWithBranches)
{
   // generate the tree
   TreeParams params;
   params.maxTreeDepth = 2;
   params.startWidth = 40;
   params.segmentInitialLength = 10;
   params.branchProbab.push_back(100);
   params.startDirection = D3DXVECTOR3(0, 1, 0);

   TreeStructureGenerator gen;
   TreeSegment* treeSkel = gen.generate(params);
   CPPUNIT_ASSERT(treeSkel != NULL);

   // prepare the expected results
   TreeSegment* parent = NULL; 
   TreeSegment* expectedStructure = new TreeSegment();
   expectedStructure->direction = params.startDirection;
   expectedStructure->position = D3DXVECTOR3(0, 0, 0);
   expectedStructure->width = 40;
   expectedStructure->type = BRANCH_START;
   expectedStructure->segmentIdx = 0;
   parent = expectedStructure;

   TreeSegment* child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 20;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parent->children.push_back(child);
   {
      TreeSegment* branch = new TreeSegment();
      branch->direction = params.startDirection;
      branch->position = D3DXVECTOR3(0, 10, 0);
      branch->width = 15;
      branch->type = BRANCH_START;
      branch->segmentIdx = 1;
      child->children.push_back(branch);

      TreeSegment* branchChild = new TreeSegment();
      branchChild->direction = params.startDirection;
      branchChild->position = D3DXVECTOR3(0, 20, 0);
      branchChild->width = 0;
      branchChild->type = BRANCH_END;
      branchChild->segmentIdx = 2;
      branch->children.push_back(branchChild);
   }
   parent = child;

   child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 2;
   parent->children.push_back(child);


   // run the test
   TreeStructureWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedStructure), writer(*treeSkel));

   delete expectedStructure;
   delete treeSkel;
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeStructureGenerator, newBranchDirectionVariation)
{
   // generate the tree
   TreeParams params;
   params.maxTreeDepth = 5;
   params.startWidth = 30;
   params.segmentInitialLength = 10;
   params.minNewBranchDirVariation = 45;
   params.maxNewBranchDirVariation = 60;
   params.branchProbab.push_back(100);
   params.startDirection = D3DXVECTOR3(0, 1, 0);

   TreeStructureGenerator gen;
   TreeSegment* treeSkel = gen.generate(params);
   CPPUNIT_ASSERT(treeSkel != NULL);
   CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int>(1), treeSkel->children.size());

   std::deque<std::pair<TreeSegment*, TreeSegment*> > segsQueue;
   segsQueue.push_back(std::make_pair(treeSkel, treeSkel->children.back()));

   while(segsQueue.size() > 0)
   {
      TreeSegment* parentSeg = segsQueue.front().first;
      TreeSegment* analyzedSeg = segsQueue.front().second;
      segsQueue.pop_front();

      if (analyzedSeg->type == BRANCH_START)
      {
         CPPUNIT_ASSERT(parentSeg->direction != analyzedSeg->direction);
      
         float dirVariationAngle = D3DXVec3Dot(&(analyzedSeg->direction), &(parentSeg->direction));
         dirVariationAngle = D3DXToDegree(acos(dirVariationAngle));
         CPPUNIT_ASSERT(dirVariationAngle >= params.minNewBranchDirVariation);
         CPPUNIT_ASSERT(dirVariationAngle <= params.maxNewBranchDirVariation);
      }

      CPPUNIT_ASSERT(analyzedSeg->children.size() <= 2);
      if (analyzedSeg->children.size() > 0)
      {
         for (std::list<TreeSegment*>::const_iterator it = analyzedSeg->children.begin();
              it != analyzedSeg->children.end(); ++it)
         {
            segsQueue.push_back(std::make_pair(analyzedSeg, *it));
         }
      }
   }

   delete treeSkel;
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeStructureGenerator, levelWhereBranchingStarts)
{
   // generate the tree
   TreeParams params;
   params.maxTreeDepth = 3;
   params.startWidth = 30;
   params.segmentInitialLength = 10;
   params.branchProbab.push_back(100);
   params.initialBranchingLevel = 2;
   params.startDirection = D3DXVECTOR3(0, 1, 0);

   TreeStructureGenerator gen;
   TreeSegment* treeSkel = gen.generate(params);
   CPPUNIT_ASSERT(treeSkel != NULL);

   // prepare the expected results
   TreeSegment* parent = NULL; 
   TreeSegment* expectedStructure = new TreeSegment();
   expectedStructure->direction = params.startDirection;
   expectedStructure->position = D3DXVECTOR3(0, 0, 0);
   expectedStructure->width = 30;
   expectedStructure->type = BRANCH_START;
   expectedStructure->segmentIdx = 0;
   parent = expectedStructure;

   TreeSegment* child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 20;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parent->children.push_back(child);
   parent = child;

   child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 10;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 2;
   parent->children.push_back(child);
   {
      TreeSegment* branch = new TreeSegment();
      branch->direction = params.startDirection;
      branch->position = D3DXVECTOR3(0, 20, 0);
      branch->width = 7.5f;
      branch->type = BRANCH_START;
      branch->segmentIdx = 2;
      child->children.push_back(branch);

      TreeSegment* branchChild = new TreeSegment();
      branchChild->direction = params.startDirection;
      branchChild->position = D3DXVECTOR3(0, 30, 0);
      branchChild->width = 0;
      branchChild->type = BRANCH_END;
      branchChild->segmentIdx = 3;
      branch->children.push_back(branchChild);
   }
   parent = child;

   child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 30, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 3;
   parent->children.push_back(child);


   // run the test
   TreeStructureWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedStructure), writer(*treeSkel));

   delete expectedStructure;
   delete treeSkel;
}

//////////////////////////////////////////////////////////////////////////////

TEST(TreeStructureGenerator, multipleBranchesAtSameLevel)
{
   // generate the tree
   TreeParams params;
   params.maxTreeDepth = 2;
   params.startWidth = 40;
   params.segmentInitialLength = 10;
   params.branchProbab.push_back(100);
   params.branchProbab.push_back(100);
   params.startDirection = D3DXVECTOR3(0, 1, 0);

   TreeStructureGenerator gen;
   TreeSegment* treeSkel = gen.generate(params);
   CPPUNIT_ASSERT(treeSkel != NULL);

   // prepare the expected results
   TreeSegment* parent = NULL; 
   TreeSegment* expectedStructure = new TreeSegment();
   expectedStructure->direction = params.startDirection;
   expectedStructure->position = D3DXVECTOR3(0, 0, 0);
   expectedStructure->width = 40;
   expectedStructure->type = BRANCH_START;
   expectedStructure->segmentIdx = 0;
   parent = expectedStructure;

   TreeSegment* child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 20;
   child->type = BRANCH_MIDDLE;
   child->segmentIdx = 1;
   parent->children.push_back(child);
   {  // branch 1
      TreeSegment* branch = new TreeSegment();
      branch->direction = params.startDirection;
      branch->position = D3DXVECTOR3(0, 10, 0);
      branch->width = 15;
      branch->type = BRANCH_START;
      branch->segmentIdx = 1;
      child->children.push_back(branch);

      TreeSegment* branchChild = new TreeSegment();
      branchChild->direction = params.startDirection;
      branchChild->position = D3DXVECTOR3(0, 20, 0);
      branchChild->width = 0;
      branchChild->type = BRANCH_END;
      branchChild->segmentIdx = 2;
      branch->children.push_back(branchChild);
   }
   {  // branch 2
      TreeSegment* branch = new TreeSegment();
      branch->direction = params.startDirection;
      branch->position = D3DXVECTOR3(0, 10, 0);
      branch->width = 15;
      branch->type = BRANCH_START;
      branch->segmentIdx = 1;
      child->children.push_back(branch);

      TreeSegment* branchChild = new TreeSegment();
      branchChild->direction = params.startDirection;
      branchChild->position = D3DXVECTOR3(0, 20, 0);
      branchChild->width = 0;
      branchChild->type = BRANCH_END;
      branchChild->segmentIdx = 2;
      branch->children.push_back(branchChild);
   }
   parent = child;

   child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 20, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 2;
   parent->children.push_back(child);


   // run the test
   TreeStructureWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedStructure), writer(*treeSkel));

   delete expectedStructure;
   delete treeSkel;
}


//////////////////////////////////////////////////////////////////////////////

TEST(TreeStructureGenerator, abruptBranchEnd)
{
   // generate the tree
   TreeParams params;
   params.maxTreeDepth = 5;
   params.startWidth = 40;
   params.segmentInitialLength = 10;
   params.abruptBranchEndProbab = 100;
   params.startDirection = D3DXVECTOR3(0, 1, 0);

   TreeStructureGenerator gen;
   TreeSegment* treeSkel = gen.generate(params);
   CPPUNIT_ASSERT(treeSkel != NULL);

   // prepare the expected results
   TreeSegment* expectedStructure = new TreeSegment();
   expectedStructure->direction = params.startDirection;
   expectedStructure->position = D3DXVECTOR3(0, 0, 0);
   expectedStructure->width = params.startWidth;
   expectedStructure->type = BRANCH_START;
   expectedStructure->segmentIdx = 0;

   TreeSegment* child = new TreeSegment();
   child->direction = params.startDirection;
   child->position = D3DXVECTOR3(0, 10, 0);
   child->width = 0;
   child->type = BRANCH_END;
   child->segmentIdx = 1;

   expectedStructure->children.push_back(child);

   // run the test
   TreeStructureWriter writer;
   CPPUNIT_ASSERT_EQUAL(writer(*expectedStructure), writer(*treeSkel));

   delete expectedStructure;
   delete treeSkel;
}

//////////////////////////////////////////////////////////////////////////////
