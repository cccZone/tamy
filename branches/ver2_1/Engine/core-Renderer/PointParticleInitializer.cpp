#include "core-Renderer\PointParticleInitializer.h"
#include "core-Renderer\Particle.h"


///////////////////////////////////////////////////////////////////////////////

PointParticleInitializer::PointParticleInitializer(float particleSize,
                                                   float particleSizeVariation,
                                                   float initialSpeed)
      : m_particleSize(particleSize),
      m_particleSizeVariation(particleSizeVariation),
      m_initialSpeed(initialSpeed)
{

}

///////////////////////////////////////////////////////////////////////////////

void PointParticleInitializer::initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                                          Particle& particle)
{
   particle.position = D3DXVECTOR3(particleSystemGlobalMtx._41, 
                                   particleSystemGlobalMtx._42,
                                   particleSystemGlobalMtx._43);

   D3DXVECTOR3 direction(1, 0, 0);
   D3DXMATRIX rotMtx;
   float angle = ((float)rand() / (float)RAND_MAX) * D3DX_PI * 2.f;
   D3DXMatrixRotationYawPitchRoll(&rotMtx, 0, 0, angle);
   D3DXVec3TransformNormal(&direction, &direction, &rotMtx);
   particle.velocity = direction * m_initialSpeed;

   particle.size = randomizeValue(m_particleSize, m_particleSizeVariation);
}

///////////////////////////////////////////////////////////////////////////////

float PointParticleInitializer::getBoundingSphereRadius() const
{
   return m_initialSpeed;
}

///////////////////////////////////////////////////////////////////////////////
