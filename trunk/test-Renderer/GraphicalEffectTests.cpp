#include "core-TestFramework\TestFramework.h"
#include "core\Array.h"
#include "GraphicalEffectMock.h"
#include "RenderingTargetsPolicyMock.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class RenderableMock
   {
   private:
      std::vector<std::string>& m_results;

   public:
      RenderableMock(std::vector<std::string>& results)
         : m_results(results)
      {}

      void render()
      {
         m_results.push_back("Renderable::render");
      }
   };
} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEffect, singlePassTechnique)
{
   std::vector<std::string> results;

   GraphicalEffectTests::RenderingTargetsPolicyMock targetsPolicy(results);
   GraphicalEffectMock effect(results);

   RenderableMock renderable(results);
   effect.render(renderable, targetsPolicy);

   CPPUNIT_ASSERT_EQUAL((unsigned int)6, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginRendering"), results[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("setting targets for pass 0"), results[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginPass - 0"), results[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderable::render"), results[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endPass - 0"), results[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endRendering"), results[5]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEffect, multiPassTechnique)
{
   std::vector<std::string> results;

   GraphicalEffectTests::RenderingTargetsPolicyMock targetsPolicy(results);
   GraphicalEffectMock effect(results);
   effect.setPassesCount(2);

   RenderableMock renderable(results);
   effect.render(renderable, targetsPolicy);

   CPPUNIT_ASSERT_EQUAL((unsigned int)10, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginRendering"), results[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("setting targets for pass 0"), results[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginPass - 0"), results[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderable::render"), results[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endPass - 0"), results[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("setting targets for pass 1"), results[5]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginPass - 1"), results[6]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderable::render"), results[7]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endPass - 1"), results[8]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endRendering"), results[9]);
}

///////////////////////////////////////////////////////////////////////////////
