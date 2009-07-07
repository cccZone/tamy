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
      m_particles(new Array<Particle*>()),
      m_activeParticles(0),
      m_spawnInterval(0),
      m_nextSpawnTime(0),
      m_spawnTimeline(0),
      m_lifeSpan(0),
      m_lifeSpanVar(0),
      m_particleInitializer(new NullParticleInitializer()),
      m_particleAnimator(new NullParticleAnimator())
{
   for (unsigned int i = 0; i < particlesCount; ++i)
   {
      m_particles->push_back(new Particle());
   }
}

///////////////////////////////////////////////////////////////////////////////

ParticleSystem::~ParticleSystem()
{
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

void ParticleSystem::setLifeSpan(float val, float variation)
{
   m_lifeSpan = val;
   m_lifeSpanVar = variation;

   float particlesCount = (float)m_particles->size();
   m_spawnInterval = m_lifeSpan / particlesCount;
   m_nextSpawnTime = 0;
   m_spawnTimeline = 0;

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

void ParticleSystem::update(float timeElapsed)
{
   ASSERT(m_activeParticles <= m_particles->size(), "ParticleSystem data is corrupt");

   // animate the existing particles
   D3DXMATRIX systemGlobalMtx = getGlobalMtx();
   Particle* particle = NULL;
   for (unsigned int i = 0; i < m_activeParticles; ++i)
   {
      particle = (*m_particles)[i];
      particle->timeToLive -= timeElapsed;

      if (particle->timeToLive <= 0)
      {
         initializeParticle(systemGlobalMtx, *particle);
      }
      else
      {
         animateParticle(*particle, timeElapsed);
      }
   }

   // add new particles if necessary
   if (m_particles->size() > m_activeParticles)
   {  
      activateParticles(systemGlobalMtx, timeElapsed);
   }

   onUpdate();
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::initializeParticle(const D3DXMATRIX& systemGlobalMtx,
                                        Particle& particle)
{
   particle.color = m_material.getLightReflectingProperties().getDiffuseColor();

   float randomVar = (-0.5f * m_lifeSpanVar) + ((float)rand() / (float)RAND_MAX) * m_lifeSpanVar;
   particle.timeToLive = m_lifeSpan + randomVar;
   particle.lifeSpan = particle.timeToLive;

   m_particleInitializer->initialize(systemGlobalMtx, particle);
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::animateParticle(Particle& particle, float& timeElapsed)
{
   particle.position += particle.velocity * timeElapsed;
   m_particleAnimator->update(particle, timeElapsed);
}

///////////////////////////////////////////////////////////////////////////////

void ParticleSystem::activateParticles(const D3DXMATRIX& systemGlobalMtx, 
                                       float& timeElapsed)
{
   m_spawnTimeline += timeElapsed;
   if (m_spawnTimeline < m_nextSpawnTime) {return;}

   unsigned int particlesCount = m_particles->size();
   unsigned int i = 0;

   unsigned int noParticlesToSpawn = 1 + (m_spawnTimeline - m_nextSpawnTime) / m_spawnInterval;
   
   for (; (i < noParticlesToSpawn) && (m_activeParticles < particlesCount); 
        ++i, ++m_activeParticles)
   {
      initializeParticle(systemGlobalMtx, *(*m_particles)[i]);
      onActivateParticle(m_activeParticles);
   }

   m_nextSpawnTime = m_spawnTimeline + m_spawnInterval;
}

///////////////////////////////////////////////////////////////////////////////
