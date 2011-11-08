#include "core-TestFramework\TestFramework.h"
#include "core-AI\FSMController.h"
#include "core-AI\FSMState.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class StringController : public FSMController< StringController >
   {
   public:
      std::string& m_log;

   public:
      StringController( std::string& log )
         : m_log(log)
      {
      }
   };

   // -------------------------------------------------------------------------

   class IdleState : public FSMState< StringController >
   {
      DECLARE_RTTI_CLASS

   public:
      void activate()
      {
         fsm().m_log += "idle-Entered; ";
      }

      void deactivate()
      {
         fsm().m_log += "idle-Exited; ";
      }

      void execute( float timeElapsed )
      {
         fsm().m_log += "idle-Updated; ";
      }
   };
   BEGIN_RTTI( IdleState )
   END_RTTI
   REGISTER_RTTI( IdleState )

   // -------------------------------------------------------------------------

   class WalkState : public FSMState< StringController >
   {
      DECLARE_RTTI_CLASS

   public:
      void activate()
      {
         fsm().m_log += "walk-Entered; ";
      }

      void deactivate()
      {
         fsm().m_log += "walk-Exited; ";
      }

      void execute( float timeElapsed )
      {
         fsm().m_log += "walk-Updated; ";
         transitionTo< IdleState >();
      }
   };
   BEGIN_RTTI( WalkState )
   END_RTTI
   REGISTER_RTTI( WalkState )

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST( StateMachine, basicSetupAndUpdates )
{
   std::string log;

   StringController fsm( log );
   fsm.registerState< IdleState >();
   fsm.begin< IdleState >();

   CPPUNIT_ASSERT_EQUAL( std::string( "" ), log );

   fsm.update( 0 );
   CPPUNIT_ASSERT_EQUAL( std::string( "idle-Entered; idle-Updated; " ), log );
   log = "";

   fsm.update(0);
   CPPUNIT_ASSERT_EQUAL( std::string( "idle-Updated; " ), log );
}

///////////////////////////////////////////////////////////////////////////////

TEST( StateMachine, transitingBetweenStates )
{
   std::string log;

   StringController fsm( log );
   fsm.registerState< IdleState >();
   fsm.registerState< WalkState >();
   fsm.begin< WalkState >();

   CPPUNIT_ASSERT_EQUAL( std::string( "" ), log );

   fsm.update( 0 );
   CPPUNIT_ASSERT_EQUAL( std::string( "walk-Entered; walk-Updated; " ), log );
   log = "";

   fsm.update(0);
   CPPUNIT_ASSERT_EQUAL( std::string( "walk-Exited; idle-Entered; idle-Updated; " ), log );
   log = "";

   fsm.transitionTo< WalkState >();
   fsm.update(0);
   CPPUNIT_ASSERT_EQUAL( std::string( "idle-Exited; walk-Entered; walk-Updated; " ), log );
}

///////////////////////////////////////////////////////////////////////////////

