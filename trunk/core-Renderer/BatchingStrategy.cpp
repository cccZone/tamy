#include "core-Renderer\BatchingStrategy.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Material.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

bool BatchComparator::operator()(AbstractGraphicalNode* lhs, 
                                 AbstractGraphicalNode* rhs) const
{
   return lhs->getMaterial() < rhs->getMaterial();
}

///////////////////////////////////////////////////////////////////////////////
