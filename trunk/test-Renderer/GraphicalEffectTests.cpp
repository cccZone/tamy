#include "core-TestFramework\TestFramework.h"
#include "core\Array.h"
#include "RenderableMock.h"
#include "GraphicalEffectMock.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEffect, singlePassTechnique)
{
   std::vector<std::string> results;

   GraphicalEffectTests::RenderingTargetsPolicyMock targetsPolicy(results);
   GraphicalEffectMock effect(targetsPolicy, new EffectDataSourceMock(results), results);

   RenderableMock renderable(results);
   effect.render(renderable);

   CPPUNIT_ASSERT_EQUAL((unsigned int) 7, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("EffectDataSource::setData"), results[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginRendering"), results[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("setting targets for pass 0"), results[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginPass - 0"), results[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderable::render"), results[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endPass - 0"), results[5]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endRendering"), results[6]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEffect, multiPassTechnique)
{
   std::vector<std::string> results;

   GraphicalEffectTests::RenderingTargetsPolicyMock targetsPolicy(results);
   GraphicalEffectMock effect(targetsPolicy, new EffectDataSourceMock(results), results);
   effect.setPassesCount(2);

   RenderableMock renderable(results);
   effect.render(renderable);

   CPPUNIT_ASSERT_EQUAL((unsigned int) 11, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("EffectDataSource::setData"), results[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginRendering"), results[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("setting targets for pass 0"), results[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginPass - 0"), results[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderable::render"), results[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endPass - 0"), results[5]);
   CPPUNIT_ASSERT_EQUAL(std::string("setting targets for pass 1"), results[6]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginPass - 1"), results[7]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderable::render"), results[8]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endPass - 1"), results[9]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endRendering"), results[10]);
}

///////////////////////////////////////////////////////////////////////////////
