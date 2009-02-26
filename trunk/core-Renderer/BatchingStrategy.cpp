#include "BatchingStrategy.h"
#include "GraphicalNode.h"
#include "Material.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

bool BatchComparator::operator()(const GraphicalNode* lhs, 
                                 const GraphicalNode* rhs) const
{
   assert(lhs != NULL);
   assert(rhs != NULL);

   return (lhs->getMaterial() < rhs->getMaterial());
}

///////////////////////////////////////////////////////////////////////////////
