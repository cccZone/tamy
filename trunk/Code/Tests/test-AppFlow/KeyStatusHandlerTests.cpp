#include "core-TestFramework\TestFramework.h"
#include "core-AppFlow\KeysStatusModel.h"
#include "core-AppFlow\KeyStatusHandler.h"
#include "core-AppFlow\KeysStatusManager.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   enum KeyOperation
   {
      KEY_UNTOUCHED,
      KEY_SMASHED,
      KEY_HELD,
      KEY_RELEASED
   };

   // -------------------------------------------------------------------------

   class KeyStatusHandlerMock : public KeyStatusHandler
   {
   private:
      KeyOperation m_lastOp;

   public:
      KeyStatusHandlerMock() : m_lastOp(KEY_UNTOUCHED)
      {}

      void keySmashed(unsigned char keyCode) {m_lastOp = KEY_SMASHED;}

      void keyHeld(unsigned char keyCode) {m_lastOp = KEY_HELD;}

      void keyReleased(unsigned char keyCode) {m_lastOp = KEY_RELEASED;}

      KeyOperation getLastOperation()
      {
         KeyOperation op = m_lastOp;
         m_lastOp = KEY_UNTOUCHED;
         return op;
      }
   };

   // -------------------------------------------------------------------------

   class KeysStatusModelMock : public KeysStatusModel
   {
   private:
      std::vector<bool> m_keyStatus;

   public:
      KeysStatusModelMock() : m_keyStatus(256, false) {}

      bool isKeyPressed(unsigned char keyCode) const
      {
         return m_keyStatus[keyCode];
      }

      void setKey(unsigned char keyCode, bool pressed)
      {
         m_keyStatus[keyCode] = pressed;
      }
   };

} // anonymous
 
///////////////////////////////////////////////////////////////////////////////

TEST( KeyStatusHandler, smashing )
{
   KeysStatusModelMock keysModel;
   KeysStatusManager keysStatusMgr(keysModel);
   KeyStatusHandlerMock* handler = new KeyStatusHandlerMock();
   keysStatusMgr.addHandler(handler);


   keysModel.setKey('A', true);
   keysStatusMgr.update(1);
   CPPUNIT_ASSERT_EQUAL(KEY_UNTOUCHED, handler->getLastOperation());

   keysModel.setKey('A', false);
   keysStatusMgr.update(1);
   CPPUNIT_ASSERT_EQUAL(KEY_RELEASED, handler->getLastOperation());
}

///////////////////////////////////////////////////////////////////////////////

TEST( KeyStatusHandler, holdingAndReleasing )
{
   KeysStatusModelMock keysModel;
   KeysStatusManager keysStatusMgr(keysModel);
   KeyStatusHandlerMock* handler = new KeyStatusHandlerMock();
   keysStatusMgr.addHandler(handler);

   keysModel.setKey('A', true);
   keysStatusMgr.update(1);
   CPPUNIT_ASSERT_EQUAL(KEY_UNTOUCHED, handler->getLastOperation());

   keysStatusMgr.update(1);
   CPPUNIT_ASSERT_EQUAL(KEY_HELD, handler->getLastOperation());

   keysStatusMgr.update(1);
   CPPUNIT_ASSERT_EQUAL(KEY_HELD, handler->getLastOperation());

   keysModel.setKey('A', false);
   keysStatusMgr.update(1);
   CPPUNIT_ASSERT_EQUAL(KEY_RELEASED, handler->getLastOperation());
}

///////////////////////////////////////////////////////////////////////////////

TEST( KeyStatusHandler, rapidSmashing )
{
   KeysStatusModelMock keysModel;
   KeysStatusManager keysStatusMgr(keysModel);
   KeyStatusHandlerMock* handler = new KeyStatusHandlerMock();
   keysStatusMgr.addHandler(handler);

   keysModel.setKey('A', true);
   keysStatusMgr.update(1);
   CPPUNIT_ASSERT_EQUAL(KEY_UNTOUCHED, handler->getLastOperation());

   keysModel.setKey('A', false);
   keysStatusMgr.update(1);
   CPPUNIT_ASSERT_EQUAL(KEY_RELEASED, handler->getLastOperation());

   keysModel.setKey('A', true);
   keysStatusMgr.update(1);
   CPPUNIT_ASSERT_EQUAL(KEY_SMASHED, handler->getLastOperation());

   keysModel.setKey('A', false);
   keysStatusMgr.update(1);
   CPPUNIT_ASSERT_EQUAL(KEY_RELEASED, handler->getLastOperation());
}

///////////////////////////////////////////////////////////////////////////////

TEST( KeyStatusHandler, smashingVsHolding )
{
   KeysStatusModelMock keysModel;
   KeysStatusManager keysStatusMgr(keysModel);
   KeyStatusHandlerMock* handler = new KeyStatusHandlerMock();
   keysStatusMgr.addHandler(handler);

   keysStatusMgr.setSmashTimeLimit(0.5f);

   keysModel.setKey('A', true);
   keysStatusMgr.update(1.0f);   // initial time doesn't matter
   CPPUNIT_ASSERT_EQUAL(KEY_UNTOUCHED, handler->getLastOperation());

   // still undecided
   keysStatusMgr.update(0.4f);
   CPPUNIT_ASSERT_EQUAL(KEY_UNTOUCHED, handler->getLastOperation());

   // ok - 0.5 sec is up - the key is clearly being held
   keysStatusMgr.update(0.1f); 
   CPPUNIT_ASSERT_EQUAL(KEY_HELD, handler->getLastOperation());

   // 1s  - the key's been released somewhere during that time
   keysModel.setKey('A', false);
   keysStatusMgr.update(0.5f);
   CPPUNIT_ASSERT_EQUAL(KEY_RELEASED, handler->getLastOperation());

   // key pressed again (the length of this period also doesn't matter)
   keysModel.setKey('A', true);
   keysStatusMgr.update(1.0f);
   CPPUNIT_ASSERT_EQUAL(KEY_SMASHED, handler->getLastOperation());

   // but after 0.4s it's been released - meaning that this was a key smash
   keysModel.setKey('A', false);
   keysStatusMgr.update(0.4f);
   CPPUNIT_ASSERT_EQUAL(KEY_RELEASED, handler->getLastOperation());
}

///////////////////////////////////////////////////////////////////////////////
