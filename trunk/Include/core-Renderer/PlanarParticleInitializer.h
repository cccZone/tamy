#pragma once

#include "core-Renderer\ParticleInitializer.h"


///////////////////////////////////////////////////////////////////////////////

class PlanarParticleInitializer : public ParticleInitializer
{
private:
   float m_planeSize;
   float m_particleSize;
   float m_particleSizeVariation;

public:
   PlanarParticleInitializer(float planeSize, 
                             float particleSize,
                             float particleSizeVariation);

   void initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                   Particle& particle);

   float getBoundingSphereRadius() const;

private:
   float randomizeValue(float max, float var) const;
};

///////////////////////////////////////////////////////////////////////////////

