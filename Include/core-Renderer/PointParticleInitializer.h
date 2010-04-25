#pragma once

#include "core-Renderer\ParticleInitializer.h"


///////////////////////////////////////////////////////////////////////////////

class PointParticleInitializer : public ParticleInitializer
{
private:
   float m_particleSize;
   float m_particleSizeVariation;
   float m_initialSpeed;

public:
   PointParticleInitializer(float particleSize,
                            float particleSizeVariation,
                            float initialSpeed);

   void initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                   Particle& particle);

   float getBoundingSphereRadius() const;
};

///////////////////////////////////////////////////////////////////////////////
