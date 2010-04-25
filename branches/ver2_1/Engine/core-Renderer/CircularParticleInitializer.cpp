#include "core-Renderer\CircularParticleInitializer.h"
#include "core-Renderer\Particle.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

CircularParticleInitializer::CircularParticleInitializer(float radius,
                                                         float particleSize,
                                                         float particleSizeVariation,
                                                         float initialSpeed)
      : m_radius(radius),
      m_particleSize(particleSize),
      m_particleSizeVariation(particleSizeVariation),
      m_initialSpeed(initialSpeed)
{
}

///////////////////////////////////////////////////////////////////////////////

void CircularParticleInitializer::initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                                             Particle& particle)
{
   D3DXVECTOR3 planeNormal(particleSystemGlobalMtx._21, 
                           particleSystemGlobalMtx._22,
                           particleSystemGlobalMtx._23);
   D3DXVECTOR3 planeCenter(particleSystemGlobalMtx._41, 
                           particleSystemGlobalMtx._42,
                           particleSystemGlobalMtx._43);

   // calculate the position on the circle's circumference
   float angle = randomizeValue(0, 2.f * D3DX_PI);
   D3DXVECTOR3 offset(sin(angle) * m_radius,
                      0,
                      cos(angle) * m_radius);
   D3DXVec3TransformCoord(&offset, &offset, &particleSystemGlobalMtx);

   particle.position = offset;
   particle.velocity = planeNormal * m_initialSpeed;
   particle.size = randomizeValue(m_particleSize, m_particleSizeVariation);
}

///////////////////////////////////////////////////////////////////////////////

float CircularParticleInitializer::getBoundingSphereRadius() const
{
   return (m_radius > m_initialSpeed) ? m_radius : m_initialSpeed;
}

///////////////////////////////////////////////////////////////////////////////
