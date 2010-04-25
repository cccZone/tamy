#pragma once

#include "core-AI\Messagable.h"


///////////////////////////////////////////////////////////////////////////////

class MessagableMock : public Messagable
{
private:
   int m_messageReceived;

public:
   MessagableMock()
   {
      m_messageReceived = -1;
   }

   RECEIVE_MESSAGES_FOR(MessagableMock);

   void setMessageVal(int val)
   {
      m_messageReceived = val;
   }

   int getMessageReceived() 
   {
      int result = m_messageReceived;
      m_messageReceived = -1;
      return result;
   }
};

///////////////////////////////////////////////////////////////////////////////

class MessageMock : public Message,
                    public TMessage<MessagableMock>
{
private:
   int m_value;

public:
   MessageMock(int val) : m_value(val) {}

   Message* clone() {return new MessageMock(*this);}

   void perform(MessagableMock& receiver, Messagable& sender)
   {
      receiver.setMessageVal(m_value);
   }
};

///////////////////////////////////////////////////////////////////////////////
