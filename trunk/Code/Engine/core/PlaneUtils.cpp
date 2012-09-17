#include "core/PlaneUtils.h"
#include "core/Plane.h"
#include "core/Vector.h"


///////////////////////////////////////////////////////////////////////////////

bool PlaneUtils::calculatePlanesIntersection( const Plane& p1, const Plane& p2, const Plane& p3, Vector& outIntersectionPt )
{
   Vector n1, n2, n3;
   p1.getNormal( n1 );
   p2.getNormal( n2 );
   p3.getNormal( n3 );

   Vector n23, n31, n12;
   n23.setCross( n2, n3 );
   n31.setCross( n3, n1 );
   n12.setCross( n1, n2 );

   float den = -p1.dotNormal( n23 );
   if ( den == 0 )
   {
      // planes are parallel
      return false;
   }

   Vector a, b, c;
   a.setMul( n23, p1.d );
   b.setMul( n31, p2.d );
   c.setMul( n12, p3.d );

   outIntersectionPt.setAdd( a, b ).add( c ).mul( 1.0f / den );
   return true;
}

///////////////////////////////////////////////////////////////////////////////
