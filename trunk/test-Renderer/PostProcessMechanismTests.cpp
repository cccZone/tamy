#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\PostProcessMechanism.h"
#include "GraphicalEffectMock.h"
#include "PostProcessEffectRenderableMock.h"
#include "RenderingTargetsPolicyMock.h"
#include <vector>
#include <string>


using namespace PostProcessMechanismTests;

///////////////////////////////////////////////////////////////////////////////

TEST(PostProcessMechanism, renderingSingleEffect)
{
   std::vector<std::string> callSequence;

   GraphicalEffectMock effect(new EffectDataSourceMock(callSequence), callSequence);
   PostProcessMechanism mechanism(new RegularTests::RenderingTargetsPolicyMock(), 
                                  effect,
                                  new PostProcessEffectRenderableMock("effect", callSequence));

   mechanism.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)6, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("EffectDataSource::setData"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginRendering"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginPass - 0"), callSequence[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("effect - render"), callSequence[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endPass - 0"), callSequence[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endRendering"), callSequence[5]);
}

///////////////////////////////////////////////////////////////////////////////
