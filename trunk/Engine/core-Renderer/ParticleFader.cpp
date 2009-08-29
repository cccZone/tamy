#include "core-Renderer\ParticleFader.h"
#include "core-Renderer\Particle.h"
#include <math.h>



///////////////////////////////////////////////////////////////////////////////

void ParticleFader::update(Particle& particle, float timeElapsed)
{
   particle.color.a = particle.timeToLive / particle.lifeSpan;
}

///////////////////////////////////////////////////////////////////////////////

float ParticleFader::getBoundingSphereRadius(float particleRange) const
{
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
