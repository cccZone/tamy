#include "BatchingStrategy.h"
#include "AbstractGraphicalNode.h"
#include "Material.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

bool BatchComparator::operator()(const AbstractGraphicalNode* lhs, 
                                 const AbstractGraphicalNode* rhs) const
{
   assert(lhs != NULL);
   assert(rhs != NULL);

   return (lhs->getMaterial() < rhs->getMaterial());
}

///////////////////////////////////////////////////////////////////////////////
