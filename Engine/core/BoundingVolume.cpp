#include "core\BoundingVolume.h"


///////////////////////////////////////////////////////////////////////////////

PlaneClassification BoundingVolume::classifyAgainsPlane(const D3DXPLANE& plane) const
{
   float dist = distanceToPlane(plane);
   if (dist < 0)
   {
      return PPC_BACK;
   }
   else if (dist > 0)
   {
      return PPC_FRONT;
   }
   else
   {
      if (hasVolume())
      {
         return PPC_SPANNING;
      }
      else
      {
         return PPC_COPLANAR;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
