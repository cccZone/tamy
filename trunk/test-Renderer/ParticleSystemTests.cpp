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

   particleSystem.update(0.25f);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, particleSystem.getActiveParticlesCount());

   particleSystem.update(0.25f);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, particleSystem.getActiveParticlesCount());

   particleSystem.update(0.25f);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, particleSystem.getActiveParticlesCount());

   // we reached the particles limit - we're not producing any more
   particleSystem.update(5);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, particleSystem.getActiveParticlesCount());
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
