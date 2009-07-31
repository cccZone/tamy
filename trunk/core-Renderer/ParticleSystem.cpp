#include "core-Renderer\ParticleSystem.h"
#include "core-Renderer\Particle.h"
#include "core-Renderer\ParticleInitializer.h"
#include "core-Renderer\ParticleAnimator.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\LightReflectingProperties.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem(const std::string& name, 
                               bool isDynamic, 
                               Material& material,
                               unsigned int particlesCount)
      : AbstractGraphicalNode(name, isDynamic, material, 0),
      m_particles(new Array<Particle*>(particlesCount)),
      m_desiredParticlesCount(particlesCount),
      m_spawnInterval(0),
      m_nextSpawnTime(0),
      m_spawnTimeline(0),
      m_lifeSpan(0),
      m_lifeSpanVar(0),
      m_loopedMode(new LoopedMode()),
      m_impulseMode(new ImpulseMode(m_desiredParticlesCount)),
      m_currentMode(m_loopedMode),
      m_particleInitializer(new NullParticleInitializer()),
      m_particleAnimator(new NullParticleAnimator())
{
}

///////////////////////////////////////////////////////////////////////////////

ParticleSystem::~ParticleSystem()
{
   delete m_loopedMode;
   m_loopedMode = NULL;

   delete m_impulseMode;
   m_impulseMode = NULL;

   m_currentMode = NULL;

   delete m_particleAnimator;
   m_particleAnimator = NULL;

   delete m_particleInitializer;
   m_particleInitializer = NULL;

   unsigned int particlesCount = m_particles->size();
   for (unsigned int i = 0; i < particlesCount; ++i)
   {
      delete (*m_particles)[i];
   }

   delete m_particles;
   m_particles = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::setLooped(bool val)
{
   m_currentMode = val ? m_loopedMode : m_impulseMode;
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::setEmissionTime(float time)
{
   if (time < 0) {time = 0;}

   m_spawnInterval = time / m_desiredParticlesCount;
   m_nextSpawnTime = 0;
   m_spawnTimeline = 0;
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::setLifeSpan(float val, float variation)
{
   if (val < 0) {val = 0;}
   if (variation < 0) {variation = 0;}

   m_lifeSpan = val;
   m_lifeSpanVar = variation;

   updateBoundingVolume();
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::setParticleInitializer(ParticleInitializer* initializer)
{
   if (initializer == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a ParticleInitializer instance");
   }

   delete m_particleInitializer;
   m_particleInitializer = initializer;

   updateBoundingVolume();
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::setParticleAnimator(ParticleAnimator* animator)
{
   if (animator == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a ParticleAnimator instance");
   }

   delete m_particleAnimator;
   m_particleAnimator = animator;

   updateBoundingVolume();
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::updateBoundingVolume()
{
   float initialRadius = m_particleInitializer->getBoundingSphereRadius();
   float speedRadius = m_particleAnimator->getBoundingSphereRadius(m_lifeSpan + m_lifeSpanVar * 0.5f);
   float radius = (initialRadius > speedRadius) ? initialRadius : speedRadius;
   setBoundingSphereRadius(radius * 1.1f);
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::activate()
{
   m_currentMode->activate();
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::deactivate()
{
   m_currentMode->deactivate();
}

///////////////////////////////////////////////////////////////////////////////

bool ParticleSystem::isActive() const
{
   return m_currentMode->isActive();
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::update(float timeElapsed)
{
   unsigned int activeParticlesCount = m_particles->size();

   // animate the existing particles
   Particle* particle = NULL;
   for (unsigned int i = 0; i < activeParticlesCount;)
   {
      particle = (*m_particles)[i];
      particle->timeToLive -= timeElapsed;

      if (particle->timeToLive > 0)
      {
         animateParticle(*particle, timeElapsed);
         ++i;
      }
      else
      {
         delete particle;
         m_particles->remove(i);
         activeParticlesCount--;
      }
   }

   // emit particles if necessary
   if (isActive())
   {
      m_spawnTimeline += timeElapsed;
   }

   if ((m_particles->size() < m_desiredParticlesCount) && (m_spawnTimeline >= m_nextSpawnTime))
   {
      m_currentMode->activateParticles(*this);
   }
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::animateParticle(Particle& particle, float& timeElapsed)
{
   particle.position += particle.velocity * timeElapsed;
   m_particleAnimator->update(particle, timeElapsed);
}

///////////////////////////////////////////////////////////////////////////////

unsigned int ParticleSystem::activateParticles()
{
   const D3DXMATRIX& systemGlobalMtx = getGlobalMtx();

   unsigned int particlesCount = m_particles->size();
   unsigned int i = 0;

   unsigned int noParticlesToSpawn = 0;
   if (m_spawnInterval == 0)
   {
      noParticlesToSpawn = m_desiredParticlesCount;
   }
   else
   {
      noParticlesToSpawn = (float)(m_spawnTimeline - m_nextSpawnTime) / (float)m_spawnInterval;
      ++noParticlesToSpawn;
   }

   unsigned int particlesMissingCount = (m_desiredParticlesCount - particlesCount);
   if (noParticlesToSpawn > particlesMissingCount)
   {
      noParticlesToSpawn = particlesMissingCount;
   }
   
   for (; (i < noParticlesToSpawn); ++i, ++particlesCount)
   {
      Particle* particle = new Particle();
      initializeParticle(systemGlobalMtx, *particle);
      m_particles->push_back(particle);
   }

   m_nextSpawnTime = m_spawnTimeline + m_spawnInterval;

   return noParticlesToSpawn;
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::initializeParticle(const D3DXMATRIX& systemGlobalMtx,
                                        Particle& particle)
{
   // Currently the particle system can operate on colors of the material only.
   // Should a different rendering material be used for a particle system,
   // a default particle color (white) will be used
   // Refactor it if it turns out to be a problem
   Material& mat = getMaterial();
   particle.color = mat.getLightReflectingProperties().getDiffuseColor();

   float randomVar = (-0.5f * m_lifeSpanVar) + ((float)rand() / (float)RAND_MAX) * m_lifeSpanVar;
   particle.timeToLive = m_lifeSpan + randomVar;
   particle.lifeSpan = particle.timeToLive;

   m_particleInitializer->initialize(systemGlobalMtx, particle);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ParticleSystem::ImpulseMode::ImpulseMode(unsigned int maxParticlesCount)
      : m_maxParticlesCount(maxParticlesCount),
      m_emittedParticlesCount(0)
{
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::ImpulseMode::activate()
{
   if (m_emittedParticlesCount >= m_maxParticlesCount)
   {
      m_emittedParticlesCount = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::ImpulseMode::deactivate()
{
   m_emittedParticlesCount = m_maxParticlesCount;
}

///////////////////////////////////////////////////////////////////////////////

bool ParticleSystem::ImpulseMode::isActive() const
{
   return (m_emittedParticlesCount < m_maxParticlesCount);
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::ImpulseMode::activateParticles(ParticleSystem& system)
{
   if (m_emittedParticlesCount >= m_maxParticlesCount) 
   {
      return;
   }

   m_emittedParticlesCount += system.activateParticles();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ParticleSystem::LoopedMode::LoopedMode()
      : m_active(true)
{
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::LoopedMode::activate()
{
   m_active = true;
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::LoopedMode::deactivate()
{
   m_active = false;
}

///////////////////////////////////////////////////////////////////////////////

bool ParticleSystem::LoopedMode::isActive() const
{
   return m_active;
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::LoopedMode::activateParticles(ParticleSystem& system)
{
   if (m_active)
   {
      system.activateParticles();
   }
}

///////////////////////////////////////////////////////////////////////////////
