#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\OverlayRenderingMechanism.h"
#include "core-Renderer\Overlay.h"
#include "RenderingTargetsPolicyMock.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class OverlayMock : public Overlay
   {
   private:
      std::vector<std::string>& m_callLog;

   public:
      OverlayMock(std::vector<std::string>& callLog)
         : m_callLog(callLog)
      {}

      void render()
      {
         m_callLog.push_back("Rendering overlay");
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(OverlayRenderingMechanism, rendering)
{
   std::vector<std::string> callLog;

   OverlayRenderingMechanism mechanism(new CallSeqTrack::RenderingTargetsPolicyMock(callLog));

   mechanism.setOverlay(new OverlayMock(callLog));

   mechanism.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, callLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Setting rendering targets"), callLog[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("Rendering overlay"), callLog[1]);
}

///////////////////////////////////////////////////////////////////////////////
