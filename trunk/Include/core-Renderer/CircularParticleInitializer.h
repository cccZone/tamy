#pragma once

#include "core-Renderer\ParticleInitializer.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This particle emitter takes the shape of a circle aligned
 * with the particle system's global transfromation
 * and will emit particles from the circumference
 * of the circle in the particle system's look vector's direction
 */
class CircularParticleInitializer : public ParticleInitializer
{
private:
   float m_radius;
   float m_particleSize;
   float m_particleSizeVariation;
   float m_initialSpeed;

public:
   CircularParticleInitializer(float radius, 
                               float particleSize,
                               float particleSizeVariation,
                               float initialSpeed);

   void initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                   Particle& particle);

   float getBoundingSphereRadius() const;
};

///////////////////////////////////////////////////////////////////////////////

