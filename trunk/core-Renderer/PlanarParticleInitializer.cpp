#include "core-Renderer\PlanarParticleInitializer.h"
#include "core-Renderer\Particle.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

PlanarParticleInitializer::PlanarParticleInitializer(float planeSize,
                                                     float particleSize,
                                                     float particleSizeVariation)
      : m_planeSize(planeSize),
      m_particleSize(particleSize),
      m_particleSizeVariation(particleSizeVariation)
{
}

///////////////////////////////////////////////////////////////////////////////

float PlanarParticleInitializer::randomizeValue(float max, float var) const
{
   float val = (-0.5f * var) + ((float)rand() / (float)RAND_MAX) * var;
   return max + val;
}

///////////////////////////////////////////////////////////////////////////////

void PlanarParticleInitializer::initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                                           Particle& particle)
{
   D3DXVECTOR3 planeCenter(particleSystemGlobalMtx._41, 
                           particleSystemGlobalMtx._42,
                           particleSystemGlobalMtx._43);

   D3DXVECTOR3 offset(randomizeValue(0, m_planeSize),
                      0,
                      randomizeValue(0, m_planeSize));

   particle.position = planeCenter + offset;

   particle.size = randomizeValue(m_particleSize, m_particleSizeVariation);
}

///////////////////////////////////////////////////////////////////////////////

float PlanarParticleInitializer::getBoundingSphereRadius() const
{
   return m_planeSize;
}

///////////////////////////////////////////////////////////////////////////////
