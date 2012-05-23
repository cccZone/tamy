#include "core\BoundingVolume.h"
#include "core\Plane.h"


///////////////////////////////////////////////////////////////////////////////

PlaneClassification BoundingVolume::classifyAgainsPlane(const Plane& plane) const
{
   float dist = distanceToPlane(plane);
   if ( dist < 0 )
   {
      return PPC_BACK;
   }
   else if ( dist > 0 )
   {
      return PPC_FRONT;
   }
   else
   {
      if ( hasVolume() )
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
