#include "core-TestFramework\TestFramework.h"
#include "core-AI\StateMachine.h"
#include "StateMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(StateMachine, transitingBetweenStates)
{
   std::string log;

   StateMachine<std::string> fsm;
   StateMock idle(log, "idle");
   StateMock walk(log, "walk");

   fsm.setState(idle);
   CPPUNIT_ASSERT_EQUAL(std::string("idle-Entered; "), log);
   log = "";

   fsm.update();
   CPPUNIT_ASSERT_EQUAL(std::string("idle-Updated; "), log);
   log = "";

   fsm.setState(walk);
   CPPUNIT_ASSERT_EQUAL(std::string("idle-Exited; walk-Entered; "), log);
   log = "";

   fsm.update();
   CPPUNIT_ASSERT_EQUAL(std::string("walk-Updated; "), log);
}

///////////////////////////////////////////////////////////////////////////////

