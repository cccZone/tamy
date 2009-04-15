#include "core-Renderer\BatchingStrategy.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Material.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

bool BatchComparator::operator()(AbstractGraphicalNode* lhs, 
                                 AbstractGraphicalNode* rhs) const
{
   Material& mat1 = lhs->getMaterial();
   Material& mat2 = rhs->getMaterial();

   return mat1.getIndex() < mat2.getIndex();
}

///////////////////////////////////////////////////////////////////////////////
