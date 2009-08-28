#include "core-Renderer\BatchingStrategy.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\Material.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

bool BatchComparator::operator()(Renderable* lhs, 
                                 Renderable* rhs) const
{
   return lhs->getMaterial() < rhs->getMaterial();
}

///////////////////////////////////////////////////////////////////////////////
