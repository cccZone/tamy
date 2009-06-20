#include "core-Renderer\LinearParticleAnimator.h"
#include "core-Renderer\Particle.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

LinearParticleAnimator::LinearParticleAnimator(const D3DXVECTOR3& velocity)
      : m_velocity(velocity),
      m_speed(D3DXVec3Length(&m_velocity))
{  
}

///////////////////////////////////////////////////////////////////////////////

void LinearParticleAnimator::update(Particle& particle, float timeElapsed)
{
   particle.velocity = m_velocity;
   particle.color.a = particle.timeToLive / particle.lifeSpan;
}

///////////////////////////////////////////////////////////////////////////////

float LinearParticleAnimator::getBoundingSphereRadius(float particleRange) const
{
   return particleRange * m_speed;
}

///////////////////////////////////////////////////////////////////////////////
