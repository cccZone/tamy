#pragma once

#include <d3dx9.h>
#include <list>


//////////////////////////////////////////////////////////////////////////////

enum TreeSegmentType
{
   BRANCH_START,
   BRANCH_MIDDLE,
   BRANCH_END
};

//////////////////////////////////////////////////////////////////////////////

/**
 * An instance of this class represents a single segment of a tree skeleton
 */
struct TreeSegment
{
public:
   std::list<TreeSegment*> children;
   D3DXVECTOR3 direction;
   D3DXVECTOR3 position;
   float width;
   TreeSegmentType type;
   unsigned int segmentIdx;

private:
   float initWidth;
   D3DXVECTOR3 rightVec;

public:
   TreeSegment();
   TreeSegment(const TreeSegment& rhs);
   virtual ~TreeSegment();

   const std::list<TreeSegment*>& getChildren() const {return children;}

private:
   friend class TreeStructureGenerator;
};

//////////////////////////////////////////////////////////////////////////////
