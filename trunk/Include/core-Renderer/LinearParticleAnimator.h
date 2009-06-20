#pragma once

#include "core-Renderer\ParticleAnimator.h"


///////////////////////////////////////////////////////////////////////////////

class LinearParticleAnimator : public ParticleAnimator
{
private:
   D3DXVECTOR3 m_velocity;
   float m_speed;

public:
   LinearParticleAnimator(const D3DXVECTOR3& velocity);

   void update(Particle& particle, float timeElapsed);

   float getBoundingSphereRadius(float particleRange) const;
};

///////////////////////////////////////////////////////////////////////////////
