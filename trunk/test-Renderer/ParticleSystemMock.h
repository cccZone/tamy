#pragma once

#include "core-Renderer\ParticleSystem.h"
#include "core-Renderer\ParticleInitializer.h"
#include "core-Renderer\ParticleAnimator.h"
#include "core-Renderer\Particle.h"
#include "core-Renderer\Material.h"
#include "LightReflectingPropertiesStub.h"
#include "MaterialOperationImplementationMock.h"


///////////////////////////////////////////////////////////////////////////////

class ParticleSystemMock : public ParticleSystem
{
private:
   static MaterialOperationImplementationMock s_matOpImpl;
   static Material s_material;

public:
   ParticleSystemMock(const std::string& name, bool isDynamic, unsigned int particlesCount) 
      : ParticleSystem(name, isDynamic, s_material, particlesCount)
   {}

   void render() {}

   const Particle& getParticle(unsigned int idx) const 
   {
      return ParticleSystem::getParticle(idx);
   }
};

///////////////////////////////////////////////////////////////////////////////

class ParticlePositionerMock : public ParticleInitializer
{
private:
   D3DXVECTOR3 m_offset;

public:
   ParticlePositionerMock(const D3DXVECTOR3& offset)
      : m_offset(offset)
   {}

   void initialize(const D3DXMATRIX& particleSystemGlobalMtx, 
                   Particle& particle)
   {
      D3DXVECTOR3 pos(particleSystemGlobalMtx._41, 
                      particleSystemGlobalMtx._42, 
                      particleSystemGlobalMtx._43);

      particle.position = pos + m_offset;
   }

   float getBoundingSphereRadius() const {return D3DXVec3Length(&m_offset);}
};

///////////////////////////////////////////////////////////////////////////////

class ParticleAnimatorMock : public ParticleAnimator
{
private:
   D3DXVECTOR3 m_velocity;

public:
   ParticleAnimatorMock(const D3DXVECTOR3& velocity)
      : m_velocity(velocity)
   {}

   void update(Particle& particle, float timeElapsed)
   {
      particle.position += m_velocity;
   }

   float getBoundingSphereRadius(float maxRange) const 
   {
      return maxRange * D3DXVec3Length(&m_velocity);
   }
};

///////////////////////////////////////////////////////////////////////////////
