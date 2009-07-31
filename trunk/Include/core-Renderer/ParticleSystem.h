#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"
#include <d3dx9.h>
#include "core\Array.h"
#include "core-Renderer\Particle.h"


///////////////////////////////////////////////////////////////////////////////

class ParticleInitializer;
class ParticleAnimator;
class Material;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a node that will spit out particles around
 */
class ParticleSystem : public AbstractGraphicalNode
{
private:
   class EmissionMode
   {
   public:
      virtual ~EmissionMode() {}

      virtual void activate() = 0;
      virtual void deactivate() = 0;
      virtual bool isActive() const = 0;
      virtual void activateParticles(ParticleSystem& system) = 0;
   };

   class ImpulseMode : public EmissionMode
   {
   private:
      unsigned int m_maxParticlesCount;
      unsigned int m_emittedParticlesCount;

   public:
      ImpulseMode(unsigned int maxParticlesCount);

      void activate();
      void deactivate();
      bool isActive() const;
      void activateParticles(ParticleSystem& system);
   };

   class LoopedMode : public EmissionMode
   {
   private:
      bool m_active;

   public:
      LoopedMode();

      void activate();
      void deactivate();
      bool isActive() const;
      void activateParticles(ParticleSystem& system);
   };

private:
   Array<Particle*>* m_particles;

   unsigned int m_desiredParticlesCount;
   float m_spawnInterval;
   float m_nextSpawnTime;
   float m_spawnTimeline;

   float m_lifeSpan;
   float m_lifeSpanVar;

   EmissionMode* m_loopedMode;
   EmissionMode* m_impulseMode;
   EmissionMode* m_currentMode;

   ParticleInitializer* m_particleInitializer;
   ParticleAnimator* m_particleAnimator;

public:
   ParticleSystem(const std::string& name, 
                  bool isDynamic, 
                  Material& material,
                  unsigned int particlesCount);
   virtual ~ParticleSystem();

   /**
    * When particle system runs in 'looped' mode, new particles are emitted
    * as soon as the existing ones die.
    *
    * In the 'impulse' mode, the specified amount of particles is emitted
    * only once. After all particles are emitted, the system deactivates itself.
    * Reactivating it will cause the system to reemit the particles in another impulse.
    */
   void setLooped(bool val);

   /**
    * This parameter specifies the period of time over which the specified
    * amount of particles will be emmited.
    * It you pass 0 here, all particles will be emitted in a single burst
    */
   void setEmissionTime(float time);

   /**
    * This parameter specifies the length of a particle life - how long
    * after it's emission will the system consider it active.
    *
    * @param variation specifies a maximum applicable life length variation that
    * will randomly be applied to each particle
    */
   void setLifeSpan(float val, float variation);

   /** 
    * This method allows you to inject different particle initialization
    * strategies
    */
   void setParticleInitializer(ParticleInitializer* initializer);

   /** 
    * This method allows you to inject different particle animation
    * strategies
    */
   void setParticleAnimator(ParticleAnimator* animator);

   /**
    * You need to call this method in order for the paticle system to 'live'
    */
   void update(float timeElapsed);

   /**
    * This method activates the system
    */
   void activate();

   /**
    * This method deactivates the system, preventing it from emitting new particles
    */
   void deactivate();

   /**
    * This method tells whether the system is active or not
    */
   bool isActive() const;

   /**
    * This method tells the number of active particles
    */
   unsigned int getActiveParticlesCount() const {return m_particles->size();}

protected:
   inline const Particle& getParticle(unsigned int idx) const {return *(*m_particles)[idx];}

private:
   void updateBoundingVolume();
   void initializeParticle(const D3DXMATRIX& systemGlobalMtx, Particle& particle);
   void animateParticle(Particle& particle, float& timeElapsed);
   unsigned int activateParticles();
};

///////////////////////////////////////////////////////////////////////////////
