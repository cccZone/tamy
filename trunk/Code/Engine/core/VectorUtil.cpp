#include "core.h"
#include "core\VectorUtil.h"
#include "core\Vector.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

void VectorUtil::calculatePerpendicularVector( const Vector& inVec, Vector& outPerpVec )
{
   // find the indices of (one of) the smallest component(s) and the indices of
   // the remaining two components in the vector
   int min = 0;
   int ok1 = 1;
   int ok2 = 2;

   float a0 = abs( inVec[0] );
   float a1 = abs( inVec[1] );
   float a2 = abs( inVec[2] );

   // sort the indices to make min index point to the smallest
   if( a1 < a0 )
   {
      ok1 = 0;
      min = 1;
      a0 = a1;
   }

   if(a2 < a0)
   {
      ok2 = min;
      min = 2;
   }

   outPerpVec.setZero();
   outPerpVec[ok1] = inVec[ok2];
   outPerpVec[ok2] = -inVec[ok1];
}

///////////////////////////////////////////////////////////////////////////////
