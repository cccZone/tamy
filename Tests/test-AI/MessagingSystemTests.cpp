#include "core-TestFramework\TestFramework.h"
#include "core-AI\MessageDispatcher.h"
#include "MessagableMock.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

TEST(MessagingSystem, deliveringMessages)
{
   const int NO_MESSAGE = -1;
   const int MESSAGE_1 = 1;
   const int MESSAGE_2 = 2;
   float delay = 0;

   MessagableMock sender1;
   MessagableMock receiver1;
   MessagableMock sender2;
   MessagableMock receiver2;

   MessageDispatcher dispatcher;

   dispatcher.send(sender1, receiver1, new MessageMock(MESSAGE_1), delay);
   dispatcher.send(sender2, receiver2, new MessageMock(MESSAGE_2), delay);

   CPPUNIT_ASSERT_EQUAL(NO_MESSAGE, receiver1.getMessageReceived());
   CPPUNIT_ASSERT_EQUAL(NO_MESSAGE, receiver2.getMessageReceived());

   dispatcher.update(0);
   CPPUNIT_ASSERT_EQUAL(MESSAGE_1, receiver1.getMessageReceived());
   CPPUNIT_ASSERT_EQUAL(MESSAGE_2, receiver2.getMessageReceived());
}

///////////////////////////////////////////////////////////////////////////////

TEST(MessagingSystem, deliveringMessagesWithDelay)
{
   const int NO_MESSAGE = -1;
   const int MESSAGE_1 = 1;
   const int MESSAGE_2 = 2;
   float noDelay = 0;
   float delay = 3;

   MessagableMock sender1;
   MessagableMock receiver1;
   MessagableMock sender2;
   MessagableMock receiver2;

   MessageDispatcher dispatcher;

   dispatcher.send(sender1, receiver1, new MessageMock(MESSAGE_1), noDelay);
   dispatcher.send(sender2, receiver2, new MessageMock(MESSAGE_2), delay);

   dispatcher.update(0);
   CPPUNIT_ASSERT_EQUAL(MESSAGE_1, receiver1.getMessageReceived());
   CPPUNIT_ASSERT_EQUAL(NO_MESSAGE, receiver2.getMessageReceived());

   dispatcher.update(2);
   CPPUNIT_ASSERT_EQUAL(NO_MESSAGE, receiver1.getMessageReceived());
   CPPUNIT_ASSERT_EQUAL(NO_MESSAGE, receiver2.getMessageReceived());

   dispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(NO_MESSAGE, receiver1.getMessageReceived());
   CPPUNIT_ASSERT_EQUAL(MESSAGE_2, receiver2.getMessageReceived());
}

///////////////////////////////////////////////////////////////////////////////
