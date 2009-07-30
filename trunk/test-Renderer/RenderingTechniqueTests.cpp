#include "core-TestFramework\TestFramework.h"
#include "AbstractGraphicalNodeMock.h"
#include "RenderingTechniqueStub.h"
#include "RenderingTargetsPolicyMock.h"
#include <stdexcept>


using namespace RenderingTechniqueTests;

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingTechnique, singlePassTechnique)
{
   std::vector<std::string> results;

   RenderingTargetsPolicyMock targetsPolicy(results);
   RenderingTechniqueStub technique("technique", targetsPolicy, 1, results);
   AbstractGraphicalNodeMock node(technique);

   // run the processor
   Array<Renderable*> nodesToRender;
   nodesToRender.push_back(&node);
   technique.render(nodesToRender);

   CPPUNIT_ASSERT_EQUAL((unsigned int) 5, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("RenderingTechnique::beginRendering"), results[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("setting targets for pass 0"), results[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("RenderingTechnique::beginPass - 0"), results[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("RenderingTechnique::endPass - 0"), results[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("RenderingTechnique::endRendering"), results[4]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingTechnique, multiPassTechnique)
{
   std::vector<std::string> results;

   RenderingTargetsPolicyMock targetsPolicy(results);
   RenderingTechniqueStub technique("technique", targetsPolicy, 2, results);
   AbstractGraphicalNodeMock node(technique);

   // run the processor
   Array<Renderable*> nodesToRender;
   nodesToRender.push_back(&node);
   technique.render(nodesToRender);

   CPPUNIT_ASSERT_EQUAL((unsigned int) 8, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("RenderingTechnique::beginRendering"), results[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("setting targets for pass 0"), results[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("RenderingTechnique::beginPass - 0"), results[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("RenderingTechnique::endPass - 0"), results[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("setting targets for pass 1"), results[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("RenderingTechnique::beginPass - 1"), results[5]);
   CPPUNIT_ASSERT_EQUAL(std::string("RenderingTechnique::endPass - 1"), results[6]);
   CPPUNIT_ASSERT_EQUAL(std::string("RenderingTechnique::endRendering"), results[7]);
}

///////////////////////////////////////////////////////////////////////////////
