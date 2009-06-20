#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

struct Particle;

///////////////////////////////////////////////////////////////////////////////

/**
 * An implementor of this interface puts life into a particle,
 * making it move and change color
 */
class ParticleAnimator
{
public:
   virtual ~ParticleAnimator() {}

   /**
    * This method should change the position of a particle
    * according to some move scheme
    */
   virtual void update(Particle& particle, float timeElapsed) = 0;

   virtual float getBoundingSphereRadius(float particleRange) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

class NullParticleAnimator : public ParticleAnimator
{
public:
   void update(Particle& particle, float timeElapsed) {}

   float getBoundingSphereRadius(float particleRange) const {return 0;}
};

///////////////////////////////////////////////////////////////////////////////
