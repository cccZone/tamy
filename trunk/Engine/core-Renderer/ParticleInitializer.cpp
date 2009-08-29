#include "core-Renderer\ParticleInitializer.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

float ParticleInitializer::randomizeValue(float max, float var) const
{
   float val = (-0.5f * var) + ((float)rand() / (float)RAND_MAX) * var;
   return max + val;
}

///////////////////////////////////////////////////////////////////////////////
