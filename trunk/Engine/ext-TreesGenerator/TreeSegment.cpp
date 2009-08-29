#include "ext-TreesGenerator\TreeSegment.h"


//////////////////////////////////////////////////////////////////////////////

TreeSegment::TreeSegment()
      : direction(D3DXVECTOR3(0, 1, 0)),
      position(D3DXVECTOR3(0, 0, 0)),
      width(0),
      type(BRANCH_START),
      segmentIdx(0),
      initWidth(0),
      rightVec(D3DXVECTOR3(1, 0, 0))
{
}

//////////////////////////////////////////////////////////////////////////////

TreeSegment::TreeSegment(const TreeSegment& rhs)
      : direction(rhs.direction),
      position(rhs.position),
      width(rhs.width),
      type(rhs.type),
      segmentIdx(rhs.segmentIdx),
      initWidth(rhs.initWidth),
      rightVec(rhs.rightVec)
{
   for (std::list<TreeSegment*>::const_iterator it = rhs.children.begin();
      it != rhs.children.end(); ++it)
   {
      children.push_back(new TreeSegment(**it));
   }
}

//////////////////////////////////////////////////////////////////////////////

TreeSegment::~TreeSegment()
{
   for (std::list<TreeSegment*>::iterator it = children.begin();
      it != children.end(); ++it)
   {
      delete *it;
   }
   children.clear();
}

//////////////////////////////////////////////////////////////////////////////
