#include "core-TestFramework\TestFramework.h"
#include "core-AI\ControllersManager.h"
#include "core-AI\Controller.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class ControllerMock : public Controller
   {
   private:
      ControllersManager& m_manager;
      std::string m_id;
      std::vector<std::string>& m_callsLog;
      bool m_remove;

   public:
      ControllerMock(ControllersManager& manager,
                     std::string id,
                     std::vector<std::string>& callsLog)
         : m_manager(manager)
         , m_id(id)
         , m_callsLog(callsLog)
         , m_remove(false)
      {
      }

      void setup(BulletPhysics& engine) {}

      void destroy(BulletPhysics& engine) {}

      void update(float timeElapsed)
      {
         if (m_remove)
         {
            m_manager.removeController(*this);
         }
         else
         {
            m_callsLog.push_back(m_id);
         }
      }


      void remove()
      {
         m_remove = true;
      }

   };
} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(ControllersManager, removingControllersDuringUpdate)
{
   std::vector<std::string> callsLog;

   ControllersManager manager;
   ControllerMock* controller1 = new ControllerMock(manager, "controller1", callsLog);
   ControllerMock* controller2 = new ControllerMock(manager, "controller2", callsLog);
   ControllerMock* controller3 = new ControllerMock(manager, "controller3", callsLog);
   manager.addController(controller1);
   manager.addController(controller2);
   manager.addController(controller3);

   // a test run - let's see if all of them are being updated
   manager.update(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, callsLog.size());
   CPPUNIT_ASSERT_EQUAL((std::string)"controller1", callsLog[0]);
   CPPUNIT_ASSERT_EQUAL((std::string)"controller2", callsLog[1]);
   CPPUNIT_ASSERT_EQUAL((std::string)"controller3", callsLog[2]);

   // now let's remove one and see how the manager handles that
   controller2->remove();

   callsLog.clear();
   manager.update(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, callsLog.size());
   CPPUNIT_ASSERT_EQUAL((std::string)"controller1", callsLog[0]);
   CPPUNIT_ASSERT_EQUAL((std::string)"controller3", callsLog[1]);

   callsLog.clear();
   manager.update(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, callsLog.size());
   CPPUNIT_ASSERT_EQUAL((std::string)"controller1", callsLog[0]);
   CPPUNIT_ASSERT_EQUAL((std::string)"controller3", callsLog[1]);
}

///////////////////////////////////////////////////////////////////////////////
