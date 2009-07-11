#include "core-TestFramework\TestFramework.h"
#include "ParticleSystemMock.h"
#include "core-Renderer\Particle.h"
#include "core\MatrixWriter.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, particlesCreation)
{
   ParticleSystemMock particleSystem("ps", false, 2);

   D3DXVECTOR3 positioningOffset(0, 0, 0);
   particleSystem.setParticleInitializer(new ParticlePositionerMock(positioningOffset));
   particleSystem.setLifeSpan(1, 0);
   particleSystem.setEmissionTime(1);

   particleSystem.update(0.25f);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, particleSystem.getActiveParticlesCount());

   particleSystem.update(0.25f);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, particleSystem.getActiveParticlesCount());

   particleSystem.update(0.25f);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, particleSystem.getActiveParticlesCount());

   // we reached the particles limit - we're not producing any more
   particleSystem.update(5);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, particleSystem.getActiveParticlesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, emmisionTime)
{
   // quick particle system emits all its particles at once
   ParticleSystemMock psQuick("psQuick", false, 3);
   psQuick.setParticleInitializer(new ParticlePositionerMock(D3DXVECTOR3(0, 0, 0)));
   psQuick.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   psQuick.setLifeSpan(2, 0);
   psQuick.setEmissionTime(0);

   psQuick.update(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, psQuick.getActiveParticlesCount());

   psQuick.update(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, psQuick.getActiveParticlesCount());

   // while the slow one distributes the emmision in time - in this
   // case it will be emitting them for 3 seconds
   ParticleSystemMock psSlow("psQuick", false, 3);
   psSlow.setParticleInitializer(new ParticlePositionerMock(D3DXVECTOR3(0, 0, 0)));
   psSlow.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   psSlow.setLifeSpan(4, 0);
   psSlow.setEmissionTime(3);

   psSlow.update(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, psSlow.getActiveParticlesCount());

   psSlow.update(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, psSlow.getActiveParticlesCount());

   psSlow.update(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, psSlow.getActiveParticlesCount());

   psSlow.update(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, psSlow.getActiveParticlesCount());

   psSlow.update(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, psSlow.getActiveParticlesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, particlesCreationWhenToMuchTimePassesAndWeNeedtoCreateMoreParticlesAtOnce)
{
   ParticleSystemMock particleSystem("ps", false, 2);

   D3DXVECTOR3 positioningOffset(0, 0, 0);
   particleSystem.setParticleInitializer(new ParticlePositionerMock(positioningOffset));
   particleSystem.setLifeSpan(1, 0);

   particleSystem.update(2.25f);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, particleSystem.getActiveParticlesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, particlesCreatedWhereTheSystemIsPositioned)
{
   ParticleSystemMock particleSystem("ps", false, 1);

   D3DXVECTOR3 positioningOffset(0, 0, 0);
   particleSystem.setParticleInitializer(new ParticlePositionerMock(positioningOffset));
   particleSystem.setLifeSpan(1, 0);

   D3DXMatrixTranslation(&(particleSystem.accessLocalMtx()), 0, 0, 10);

   particleSystem.update(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, particleSystem.getActiveParticlesCount());
   COMPARE_VEC(D3DXVECTOR3(0, 0, 10), particleSystem.getParticle(0).position);
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, particlesAlternativePositioning)
{
   ParticleSystemMock particleSystem("ps", false, 1);

   D3DXVECTOR3 positioningOffset(5, 0, 0);
   particleSystem.setParticleInitializer(new ParticlePositionerMock(positioningOffset));
   particleSystem.setLifeSpan(1, 0);

   D3DXMatrixTranslation(&(particleSystem.accessLocalMtx()), 0, 0, 10);

   particleSystem.update(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, particleSystem.getActiveParticlesCount());
   COMPARE_VEC(D3DXVECTOR3(5, 0, 10), particleSystem.getParticle(0).position);
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, particlesMovement)
{
   ParticleSystemMock particleSystem("ps", false, 1);

   D3DXVECTOR3 positioningOffset(0, 0, 0);
   particleSystem.setParticleInitializer(new ParticlePositionerMock(positioningOffset));
   particleSystem.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   particleSystem.setLifeSpan(3, 0);

   particleSystem.update(3);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, particleSystem.getActiveParticlesCount());
   COMPARE_VEC(D3DXVECTOR3(0, 0, 0), particleSystem.getParticle(0).position);

   particleSystem.update(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, particleSystem.getActiveParticlesCount());
   COMPARE_VEC(D3DXVECTOR3(0, 0, 1), particleSystem.getParticle(0).position);

   particleSystem.update(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, particleSystem.getActiveParticlesCount());
   COMPARE_VEC(D3DXVECTOR3(0, 0, 2), particleSystem.getParticle(0).position);
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, boundingVolume)
{
   // testing first system
   ParticleSystemMock ps1("ps", false, 1);
   ps1.setParticleInitializer(new ParticlePositionerMock(D3DXVECTOR3(0, 0, 0)));
   ps1.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   ps1.setLifeSpan(1, 0);

   const BoundingSphere* bs = dynamic_cast<const BoundingSphere*>(&(ps1.getBoundingVolume()));
   CPPUNIT_ASSERT(bs != NULL);
   CPPUNIT_ASSERT_EQUAL(1.1f, bs->radius);
   COMPARE_VEC(D3DXVECTOR3(0, 0, 0), bs->origin);

   // testing another system - here the particles fly with the same speed,
   // but have a longer range
   ParticleSystemMock ps2("ps", false, 1);
   ps2.setParticleInitializer(new ParticlePositionerMock(D3DXVECTOR3(0, 0, 0)));
   ps2.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   ps2.setLifeSpan(10, 0);

   bs = dynamic_cast<const BoundingSphere*>(&(ps2.getBoundingVolume()));
   CPPUNIT_ASSERT(bs != NULL);
   CPPUNIT_ASSERT_EQUAL(11.f, bs->radius);
   COMPARE_VEC(D3DXVECTOR3(0, 0, 0), bs->origin);
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, singleImpulseParticlesEmission)
{
   ParticleSystemMock ps("ps", false, 10);
   ps.setParticleInitializer(new ParticlePositionerMock(D3DXVECTOR3(0, 0, 0)));
   ps.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   ps.setLifeSpan(2, 0);
   ps.setEmissionTime(2);
   ps.setLooped(false);

   ps.update(0);
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());

   ps.update(1);
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)6, ps.getActiveParticlesCount());

   ps.update(1);
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)9, ps.getActiveParticlesCount());

   ps.update(1);
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, ps.getActiveParticlesCount());

   ps.update(1);
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, ps.getActiveParticlesCount());

   ps.update(1);
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, ps.getActiveParticlesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, loopedPerticlesEmission)
{
   ParticleSystemMock ps("ps", false, 1);
   ps.setParticleInitializer(new ParticlePositionerMock(D3DXVECTOR3(0, 0, 0)));
   ps.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   ps.setLifeSpan(2, 0);
   ps.setLooped(true);

   ps.update(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());
   CPPUNIT_ASSERT_EQUAL(D3DXVECTOR3(0, 0, 0), ps.getParticle(0).position);
   CPPUNIT_ASSERT_EQUAL(2.f, ps.getParticle(0).timeToLive);

   ps.update(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());
   CPPUNIT_ASSERT_EQUAL(D3DXVECTOR3(0, 0, 1), ps.getParticle(0).position);
   CPPUNIT_ASSERT_EQUAL(1.f, ps.getParticle(0).timeToLive);

   ps.update(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());
   CPPUNIT_ASSERT_EQUAL(D3DXVECTOR3(0, 0, 0), ps.getParticle(0).position);
   CPPUNIT_ASSERT_EQUAL(2.f, ps.getParticle(0).timeToLive);
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, activationAndDeactivation_impulse)
{
   ParticleSystemMock ps("ps", false, 1);
   ps.setParticleInitializer(new ParticlePositionerMock(D3DXVECTOR3(0, 0, 0)));
   ps.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   ps.setLifeSpan(2, 0);
   ps.setLooped(false);

   // by default the particle system is active and starts spitting particles automatically
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());

   ps.update(0);
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive()); // but immediately after it emits all the particles, it becomes inactive
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());

   // after all particles died, no new particles are created and the system gets deactivated
   ps.update(2);
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, ps.getActiveParticlesCount());

   // however we can reactivate it - int that case it starts
   // spitting the particles out all over again
   ps.activate();
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());

   ps.update(0);
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, activatingParticlesDuringInputSystemActivityTime)
{
   ParticleSystemMock ps("ps", false, 4);
   ps.setParticleInitializer(new ParticlePositionerMock(D3DXVECTOR3(0, 0, 0)));
   ps.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   ps.setLifeSpan(20, 0);
   ps.setEmissionTime(4);
   ps.setLooped(false);

   // system has 4 particles to emit over the period of 4 seconds,
   // meaning that it will emmit a new particle each second.
   ps.update(0);
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());
   ps.update(1);
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, ps.getActiveParticlesCount());

   // 2 seconds passed and we have 2 particles emitted.
   // Let's see what happends when we reactivate the system
   // this instant
   ps.activate();
   ps.update(1);
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, ps.getActiveParticlesCount());
   ps.update(1);
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, ps.getActiveParticlesCount());

   // all the particles that were emitted are still alive - we reached the
   // limit of particles we can emit.
   ps.update(1);
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, ps.getActiveParticlesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, impulseActivactedAfterLongInactivity)
{
   ParticleSystemMock ps("ps", false, 4);
   ps.setParticleInitializer(new ParticlePositionerMock(D3DXVECTOR3(0, 0, 0)));
   ps.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   ps.setLifeSpan(4, 0);
   ps.setEmissionTime(4);
   ps.setLooped(false);

   // system has 4 particles to emit over the period of 4 seconds,
   // meaning that it will emmit a new particle each second.

   // let's deactivate it and wait a considerable amount of time
   ps.deactivate();
   ps.update(20);

   // now let's activate it - the emission time should be maintained
   ps.activate();
   ps.update(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());
   ps.update(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, ps.getActiveParticlesCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ParticleSystem, activationAndDeactivation_looped)
{
   
   ParticleSystemMock ps("ps", false, 1);
   ps.setParticleInitializer(new ParticlePositionerMock(D3DXVECTOR3(0, 0, 0)));
   ps.setParticleAnimator(new ParticleAnimatorMock(D3DXVECTOR3(0, 0, 1)));
   ps.setLifeSpan(2, 0);
   ps.setLooped(true);

   // by default the particle system is active and starts spitting particles automatically
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());

   ps.update(0);
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());

   ps.update(2);
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());

   // we can deactivate it - in that case the remaining active particles
   // will be depleted and no new ones will get generated
   ps.deactivate();
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());

   ps.update(2);
   CPPUNIT_ASSERT_EQUAL(false, ps.isActive());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, ps.getActiveParticlesCount());  

   // and we can bring it back to life again...

   ps.activate();
   CPPUNIT_ASSERT_EQUAL(true, ps.isActive());

   ps.update(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, ps.getActiveParticlesCount());
}

///////////////////////////////////////////////////////////////////////////////
