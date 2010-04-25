#pragma once

#include "core-Renderer\ParticleInitializer.h"


///////////////////////////////////////////////////////////////////////////////

class PlanarParticleInitializer : public ParticleInitializer
{
private:
   float m_planeSize;
   float m_particleSize;
   float m_particleSizeVariation;
   float m_initialSpeed;

public:
   PlanarParticleInitializer(float planeSize, 
                             float particleSize,
                             float particleSizeVariation,
                             float initialSpeed);

   void initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                   Particle& particle);

   float getBoundingSphereRadius() const;
};

///////////////////////////////////////////////////////////////////////////////

