#include "core-TestFramework\TestFramework.h"
#include "core-AI\FsmLogicStateController.h"
#include "core-AI\FsmLogicState.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class StringController : public FsmLogicStateController<StringController>
   {
   };

   // -------------------------------------------------------------------------

   class StateMock : public FsmLogicState<StringController>
   {
   private:
      std::string& m_log;
      std::string m_stateName;

   public:
      StateMock(std::string& log, const std::string& stateName)
      : m_log(log)
      , m_stateName(stateName)
      {
      }

      void onEnter(StringController& controller)
      {
         m_log += m_stateName + "-Entered; ";
      }

      void onExit(StringController& controller)
      {
         m_log += m_stateName + "-Exited; ";
      }

      void evaluate(StringController& controller, float timeElapsed)
      {
         m_log += m_stateName + "-Updated; ";
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(StateMachine, transitingBetweenStates)
{
   std::string log;

   StringController fsm;
   StateMock idle(log, "idle");
   StateMock walk(log, "walk");

   fsm.changeState(idle);
   CPPUNIT_ASSERT_EQUAL(std::string("idle-Entered; "), log);
   log = "";

   fsm.update(0);
   CPPUNIT_ASSERT_EQUAL(std::string("idle-Updated; "), log);
   log = "";

   fsm.changeState(walk);
   CPPUNIT_ASSERT_EQUAL(std::string("idle-Exited; walk-Entered; "), log);
   log = "";

   fsm.update(0);
   CPPUNIT_ASSERT_EQUAL(std::string("walk-Updated; "), log);
}

///////////////////////////////////////////////////////////////////////////////

