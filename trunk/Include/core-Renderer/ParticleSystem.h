#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"
#include <d3dx9.h>
#include "core\Array.h"
#include "core-Renderer\Color.h"
#include "core-Renderer\Particle.h"


///////////////////////////////////////////////////////////////////////////////

class ParticleInitializer;
class ParticleAnimator;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a node that will spit out particles around
 */
class ParticleSystem : public AbstractGraphicalNode
{
private:
   Array<Particle*>* m_particles;
   unsigned int m_activeParticles;

   float m_spawnInterval;
   float m_nextSpawnTime;
   float m_spawnTimeline;

   float m_lifeSpan;
   float m_lifeSpanVar;

   ParticleInitializer* m_particleInitializer;
   ParticleAnimator* m_particleAnimator;

public:
   ParticleSystem(const std::string& name, 
                  bool isDynamic, 
                  Material& material,
                  unsigned int particlesCount);
   virtual ~ParticleSystem();

   void setLifeSpan(float val, float variation);
   void setParticleInitializer(ParticleInitializer* initializer);
   void setParticleAnimator(ParticleAnimator* animator);

   void update(float timeElapsed);

   unsigned int getActiveParticlesCount() const {return m_activeParticles;}

protected:
   virtual void onUpdate() = 0;
   virtual void onActivateParticle(unsigned int idx) = 0;

   inline const Particle& getParticle(unsigned int idx) const {return *(*m_particles)[idx];}

private:
   void updateBoundingVolume();
   void initializeParticle(const D3DXMATRIX& systemGlobalMtx, Particle& particle);
   void animateParticle(Particle& particle, float& timeElapsed);
   void activateParticles(const D3DXMATRIX& systemGlobalMtx, float& timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////
