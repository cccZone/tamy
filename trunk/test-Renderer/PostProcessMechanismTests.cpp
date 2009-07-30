#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\PostProcessMechanism.h"
#include "RenderingTechniqueStub.h"
#include "PostProcessEffectNodeMock.h"
#include "RenderingTargetsPolicyMock.h"
#include <vector>
#include <string>


using namespace PostProcessMechanismTests;

///////////////////////////////////////////////////////////////////////////////

TEST(PostProcessMechanism, renderingSingleEffect)
{
   std::vector<std::string> callSequence;

   RenderingProcessorTests::RenderingTechniqueStub technique("technique0", callSequence);
   PostProcessMechanism mechanism(new RegularTests::RenderingTargetsPolicyMock(), 
                                  technique,
                                  new PostProcessEffectNodeMock("effect", callSequence));

   mechanism.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Set technique technique0"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("effect - render"), callSequence[1]);
}

///////////////////////////////////////////////////////////////////////////////
