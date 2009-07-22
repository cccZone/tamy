#include "core-Renderer\BatchingStrategy.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\RenderingTechnique.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

bool BatchComparator::operator()(AbstractGraphicalNode* lhs, 
                                 AbstractGraphicalNode* rhs) const
{
   return lhs->getTechnique() < rhs->getTechnique();
}

///////////////////////////////////////////////////////////////////////////////
