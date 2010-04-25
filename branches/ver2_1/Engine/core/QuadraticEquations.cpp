#include "core\QuadraticEquations.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

bool solveQuadratic(float a, float b, float c, float& firstSolution)
{
   float d, oneOverTwoA, t0, t1, temp;

   d = b*b - 4*a*c;
   if (d < 0.0f) return false;

   d = sqrtf( d );
   oneOverTwoA = 1.0f / (2.0f * a);

   t0 = (-b - d) * oneOverTwoA;
   t1 = (-b + d) * oneOverTwoA;

   if (t1 < t0) { temp = t0; t0 = t1; t1 = temp; }

   if (t1 < 0.0f) return false;

   if (t0 < 0.0f) {firstSolution = t1;} else {firstSolution = t0;}

   return true;
}

///////////////////////////////////////////////////////////////////////////////
