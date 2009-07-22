#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\CyclicRenderingTarget.h"
#include "TextureRenderingTargetMock.h"


using namespace RendereringPipelineTests; // for TextureRenderingTargetMock

///////////////////////////////////////////////////////////////////////////////

TEST(CyclicRenderingTarget, cyclicBufferSwitching)
{
   std::vector<std::string> callLog;

   CyclicRenderingTarget cyclicTarget("cyclicTarget");
   cyclicTarget.add(new TextureRenderingTargetMock("target0", callLog));
   cyclicTarget.add(new TextureRenderingTargetMock("target1", callLog));
   cyclicTarget.add(new TextureRenderingTargetMock("target2", callLog));

   // buffers change when we issue the command
   cyclicTarget.switchBuffer();
   callLog.clear();
   cyclicTarget.use(0);
   CPPUNIT_ASSERT_EQUAL(std::string("target0 - 0"), callLog[0]);

   callLog.clear();
   cyclicTarget.use(0);
   CPPUNIT_ASSERT_EQUAL(std::string("target0 - 0"), callLog[0]);

   // switch
   cyclicTarget.switchBuffer();
   callLog.clear();
   cyclicTarget.use(0);
   CPPUNIT_ASSERT_EQUAL(std::string("target1 - 0"), callLog[0]);

   cyclicTarget.switchBuffer();
   callLog.clear();
   cyclicTarget.use(0);
   CPPUNIT_ASSERT_EQUAL(std::string("target2 - 0"), callLog[0]);


   // which pass are we using them for doesn't matter - the cycle is maintained
   cyclicTarget.switchBuffer();
   callLog.clear();
   cyclicTarget.use(1);
   CPPUNIT_ASSERT_EQUAL(std::string("target0 - 1"), callLog[0]);

   cyclicTarget.switchBuffer();
   callLog.clear();
   cyclicTarget.use(5);
   CPPUNIT_ASSERT_EQUAL(std::string("target1 - 5"), callLog[0]);

   cyclicTarget.switchBuffer();
   callLog.clear();
   cyclicTarget.use(3);
   CPPUNIT_ASSERT_EQUAL(std::string("target2 - 3"), callLog[0]);
}

///////////////////////////////////////////////////////////////////////////////
