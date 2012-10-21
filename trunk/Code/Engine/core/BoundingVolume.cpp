#include "core.h"
#include "core\BoundingVolume.h"
#include "core\Plane.h"


///////////////////////////////////////////////////////////////////////////////

PlaneClassification BoundingVolume::classifyAgainsPlane(const Plane& plane) const
{
   FastFloat dist = distanceToPlane(plane);
   if ( dist < Float_0 )
   {
      return PPC_BACK;
   }
   else if ( dist > Float_0 )
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
