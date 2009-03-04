#pragma once


//////////////////////////////////////////////////////////////////////////////

struct TreeParams;
struct TreeSegment;

//////////////////////////////////////////////////////////////////////////////

/**
 * This class is responsible for generating a skelton of a tree.
 *
 * The skeleton is NOT a skeleton in the skinned mesh animation sense.
 * It's just a description of how the tree should be built, where does it branch
 * out, in what direction etc.
 */
class TreeStructureGenerator
{
public:
   TreeStructureGenerator();

   TreeSegment* generate(const TreeParams& params);

private:
   TreeSegment* createNextBranchSegment(const TreeParams& params, 
                                        TreeSegment* paren,
                                        bool newBranch);

   void variateDirection(const TreeSegment& referenceSeg, 
                         float minVarAngle, 
                         float maxVarAngle,
                         TreeSegment& changedSeg);

   inline bool randomChance(float probab) const;
};

//////////////////////////////////////////////////////////////////////////////
