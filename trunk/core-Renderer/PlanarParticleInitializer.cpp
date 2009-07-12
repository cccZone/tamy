#include "core-Renderer\PlanarParticleInitializer.h"
#include "core-Renderer\Particle.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

PlanarParticleInitializer::PlanarParticleInitializer(float planeSize,
                                                     float particleSize,
                                                     float particleSizeVariation,
                                                     float initialSpeed)
      : m_planeSize(planeSize),
      m_particleSize(particleSize),
      m_particleSizeVariation(particleSizeVariation),
      m_initialSpeed(initialSpeed)
{
}

///////////////////////////////////////////////////////////////////////////////

void PlanarParticleInitializer::initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                                           Particle& particle)
{
   D3DXVECTOR3 planeNormal(particleSystemGlobalMtx._21, 
                           particleSystemGlobalMtx._22,
                           particleSystemGlobalMtx._23);
   D3DXVECTOR3 planeCenter(particleSystemGlobalMtx._41, 
                           particleSystemGlobalMtx._42,
                           particleSystemGlobalMtx._43);

   D3DXVECTOR3 offset(randomizeValue(0, m_planeSize),
                      0,
                      randomizeValue(0, m_planeSize));
   D3DXVec3TransformCoord(&offset, &offset, &particleSystemGlobalMtx);

   particle.position = offset;
   particle.velocity = planeNormal * m_initialSpeed;
   particle.size = randomizeValue(m_particleSize, m_particleSizeVariation);
}

///////////////////////////////////////////////////////////////////////////////

float PlanarParticleInitializer::getBoundingSphereRadius() const
{
   return (m_planeSize > m_initialSpeed) ? m_planeSize : m_initialSpeed;
}

///////////////////////////////////////////////////////////////////////////////
