#pragma once

#include <d3dx9.h>
#include <list>


//////////////////////////////////////////////////////////////////////////////

/**
 * This structure contains the parameters that drive
 * the tree generation process
 */
struct TreeParams
{
   /**
    * This param specifies how many segments can a tree have.
    *
    * Each segment of a tree is a point where a ring of vertices will
    * be located. It is also the point where the branches will originate.
    */
   unsigned int   maxTreeDepth;

   /**
    * This param specifies the volume of the trunk at it's root.
    */
   float          startWidth;

   /**
    * This param specifies the direction in which the next
    * segment based in the root will be created in.
    *
    * This will usually be pointing upwards (0,1,0), but ocassionally
    * one may want to pick a different direction (if a tree grows out of the
    * cliff side for instance, the proper dir would be (1,0,0)
    */
   D3DXVECTOR3    startDirection;

   /**
    * Each segment has a certain lenght. This parameter specifies
    * the legth of the very first segment in the trunk of the tree
    */
   float          segmentInitialLength;

   /**
    * These two parameters describe the range in which the direction of
    * each subsequent branch segment change (so that the branch appears
    * twisted)
    */
   float          minInternalBranchDirVariation;
   float          maxInternalBranchDirVariation;

   /**
    * Eanch entry in this list describes the probability with which
    * an additional branch can originate at each new segment of the root branch.
    *
    * Each value should be in rage (0, 100>
    */
   std::list<float> branchProbab;

   /**
    * These two parameters describe the range in which the direction of
    * each new branch will be branched out.
    */
   float          minNewBranchDirVariation;
   float          maxNewBranchDirVariation;

   /**
    * This value specifies the segment index at which the first
    * branch can be created
    */
   unsigned int initialBranchingLevel;

   /**
    * This parameter describes the probability with which the branch will
    * end before its length reaches the maximum tree depth
    */
   float abruptBranchEndProbab;

   TreeParams()
      : maxTreeDepth(1),
      startWidth(0),
      startDirection(D3DXVECTOR3(0, 1, 0)),
      segmentInitialLength(0),
      minInternalBranchDirVariation(0),
      maxInternalBranchDirVariation(0),
      minNewBranchDirVariation(0),
      maxNewBranchDirVariation(0),
      initialBranchingLevel(0),
      abruptBranchEndProbab(0)
   {
   }
};

//////////////////////////////////////////////////////////////////////////////
