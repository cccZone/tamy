#pragma once

#include "core-Renderer\ParticleAnimator.h"


///////////////////////////////////////////////////////////////////////////////

class ParticleFader : public ParticleAnimator
{

public:
   void update(Particle& particle, float timeElapsed);

   float getBoundingSphereRadius(float particleRange) const;
};

///////////////////////////////////////////////////////////////////////////////
