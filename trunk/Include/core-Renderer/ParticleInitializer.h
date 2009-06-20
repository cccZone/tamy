#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

struct Particle;
class LightReflectingProperties;

///////////////////////////////////////////////////////////////////////////////

/**
 * An implementor of this interface can redefine the position
 * of a particle
 */
class ParticleInitializer
{
public:
   virtual ~ParticleInitializer() {}

   /**
    * This method is responsible for setting the initial values of a particle.
    */
   virtual void initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                           Particle& particle) = 0;

   virtual float getBoundingSphereRadius() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

class NullParticleInitializer : public ParticleInitializer
{
public:
   void initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                   Particle& particle) {}

   float getBoundingSphereRadius() const {return 0;}
};

///////////////////////////////////////////////////////////////////////////////
