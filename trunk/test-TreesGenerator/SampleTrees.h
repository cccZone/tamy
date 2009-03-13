#pragma once

#include "TreeSegment.h"


///////////////////////////////////////////////////////////////////////////////

class SampleTrees
{
public:
   static void higherTwoBranchTree(TreeSegment& tree)
   {
      // prepare the tree structure
      tree.direction = D3DXVECTOR3(0, 1, 0);
      tree.position = D3DXVECTOR3(0, 0, 0);
      tree.width = 10;
      tree.type = BRANCH_START;
      tree.segmentIdx = 0;
      TreeSegment* parentSeg = &tree;

      TreeSegment* child = new TreeSegment();
      child->direction = D3DXVECTOR3(0, 1, 0);
      child->position = D3DXVECTOR3(0, 10, 0);
      child->width = 10;
      child->type = BRANCH_MIDDLE;
      child->segmentIdx = 1;
      parentSeg->children.push_back(child);
      parentSeg = child;

      child = new TreeSegment();
      child->direction = D3DXVECTOR3(0, 1, 0);
      child->position = D3DXVECTOR3(0, 20, 0);
      child->width = 10;
      child->type = BRANCH_MIDDLE;
      child->segmentIdx = 2;
      parentSeg->children.push_back(child);
      parentSeg = child;

      child = new TreeSegment();
      child->direction = D3DXVECTOR3(0, 1, 0);
      child->position = D3DXVECTOR3(0, 30, 0);
      child->width = 10;
      child->type = BRANCH_MIDDLE;
      child->segmentIdx = 3;
      parentSeg->children.push_back(child);
      parentSeg = child;

      {
         TreeSegment* branchRoot = new TreeSegment();
         branchRoot->rightVec = D3DXVECTOR3(-1, 0, 0);
         branchRoot->direction = D3DXVECTOR3(0, 0, -1);
         branchRoot->position = D3DXVECTOR3(0, 30, 0);
         branchRoot->width = 10;
         branchRoot->type = BRANCH_START;
         branchRoot->segmentIdx = 3;
         parentSeg->children.push_back(branchRoot);

         TreeSegment* branchChild = new TreeSegment();
         branchChild->rightVec = D3DXVECTOR3(-1, 0, 0);
         branchChild->direction = D3DXVECTOR3(0, 0, -1);
         branchChild->position = D3DXVECTOR3(0, 30, -10);
         branchChild->width = 0;
         branchChild->type = BRANCH_END;
         branchChild->segmentIdx = 4;
         branchRoot->children.push_back(branchChild);
      }

      child = new TreeSegment();
      child->direction = D3DXVECTOR3(0, 1, 0);
      child->position = D3DXVECTOR3(0, 40, 0);
      child->width = 0;
      child->type = BRANCH_END;
      child->segmentIdx = 4;
      parentSeg->children.push_back(child);
   }

   static void highTwoBranchTree(TreeSegment& tree)
   {
      // prepare the tree structure
      tree.direction = D3DXVECTOR3(0, 1, 0);
      tree.position = D3DXVECTOR3(0, 0, 0);
      tree.width = 10;
      tree.type = BRANCH_START;
      tree.segmentIdx = 0;
      TreeSegment* parentSeg = &tree;

      TreeSegment* child = new TreeSegment();
      child->direction = D3DXVECTOR3(0, 1, 0);
      child->position = D3DXVECTOR3(0, 10, 0);
      child->width = 10;
      child->type = BRANCH_MIDDLE;
      child->segmentIdx = 1;
      parentSeg->children.push_back(child);
      parentSeg = child;

      child = new TreeSegment();
      child->direction = D3DXVECTOR3(0, 1, 0);
      child->position = D3DXVECTOR3(0, 20, 0);
      child->width = 10;
      child->type = BRANCH_MIDDLE;
      child->segmentIdx = 2;
      parentSeg->children.push_back(child);
      parentSeg = child;

      {
         TreeSegment* branchRoot = new TreeSegment();
         branchRoot->rightVec = D3DXVECTOR3(-1, 0, 0);
         branchRoot->direction = D3DXVECTOR3(0, 0, -1);
         branchRoot->position = D3DXVECTOR3(0, 20, 0);
         branchRoot->width = 10;
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
   }

   static void threeBranchTree(TreeSegment& tree)
   {
      // prepare the tree structure
      tree.direction = D3DXVECTOR3(0, 1, 0);
      tree.position = D3DXVECTOR3(0, 0, 0);
      tree.width = 10;
      tree.type = BRANCH_START;
      tree.segmentIdx = 0;
      TreeSegment* parentSeg = &tree;

      TreeSegment* child = new TreeSegment();
      child->direction = D3DXVECTOR3(0, 1, 0);
      child->position = D3DXVECTOR3(0, 10, 0);
      child->width = 10;
      child->type = BRANCH_MIDDLE;
      child->segmentIdx = 1;
      parentSeg->children.push_back(child);
      parentSeg = child;

      {
         TreeSegment* branchRoot = new TreeSegment();
         branchRoot->rightVec = D3DXVECTOR3(1, 0, 0);
         branchRoot->direction = D3DXVECTOR3(0, 0, 1);
         branchRoot->position = D3DXVECTOR3(0, 10, 0);
         branchRoot->width = 10;
         branchRoot->type = BRANCH_START;
         branchRoot->segmentIdx = 1;
         parentSeg->children.push_back(branchRoot);
         TreeSegment* branchParent = branchRoot;

         TreeSegment* branchChild = new TreeSegment();
         branchChild->rightVec = D3DXVECTOR3(1, 0, 0);
         branchChild->direction = D3DXVECTOR3(0, 0, 1);
         branchChild->position = D3DXVECTOR3(0, 10, 10);
         branchChild->width = 10;
         branchChild->type = BRANCH_MIDDLE;
         branchChild->segmentIdx = 2;
         branchParent->children.push_back(branchChild);
         branchParent = branchChild;

         {
            TreeSegment* subBranchRoot = new TreeSegment();
            subBranchRoot->rightVec = D3DXVECTOR3(1, 0, 0);
            subBranchRoot->direction = D3DXVECTOR3(0, -1, 0);
            subBranchRoot->position = D3DXVECTOR3(0, 10, 10);
            subBranchRoot->width = 10;
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
         branchChild->rightVec = D3DXVECTOR3(1, 0, 0);
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
      child->width = 10;
      child->type = BRANCH_MIDDLE;
      child->segmentIdx = 2;
      parentSeg->children.push_back(child);
      parentSeg = child;

      {
         TreeSegment* branchRoot = new TreeSegment();
         branchRoot->rightVec = D3DXVECTOR3(-1, 0, 0);
         branchRoot->direction = D3DXVECTOR3(0, 0, -1);
         branchRoot->position = D3DXVECTOR3(0, 20, 0);
         branchRoot->width = 10;
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
   }
};

///////////////////////////////////////////////////////////////////////////////
