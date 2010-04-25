#include "core-Renderer\DistanceComparator.h"
#include "core-Renderer\Renderable.h"


///////////////////////////////////////////////////////////////////////////////

bool DistanceComparator::operator()(Renderable* lhs, Renderable* rhs) const
{
   const D3DXMATRIX& mtx1 = lhs->getGlobalTransform();
   const D3DXMATRIX& mtx2 = rhs->getGlobalTransform();

   return calcDistanceTo(mtx1._41, mtx1._42, mtx1._43) > 
          calcDistanceTo(mtx2._41, mtx2._42, mtx2._43);
}

///////////////////////////////////////////////////////////////////////////////
