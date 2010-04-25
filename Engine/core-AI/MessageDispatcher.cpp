#include "core-AI\MessageDispatcher.h"
#include "core-AI\Messagable.h"


///////////////////////////////////////////////////////////////////////////////

void MessageDispatcher::send(Messagable& sender, 
                             Messagable& receiver, 
                             Message* message, float delay)
{
   m_msgQueue.push_back(Telegram(sender, receiver, message, delay));
}

///////////////////////////////////////////////////////////////////////////////

void MessageDispatcher::update(float timeElapsed)
{
   MessagesQueue::iterator it = m_msgQueue.begin();

   while(it != m_msgQueue.end())
   {
      Telegram& tlg = *it;
      tlg.delay -= timeElapsed;

      if (tlg.delay <= 0)
      {
         tlg.receiver.receiveMessage(tlg.sender, *(tlg.message));
         it = m_msgQueue.erase(it);
      }
      else
      {
         it++;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

MessageDispatcher::Telegram::Telegram(Messagable& _sender, 
                                      Messagable& _receiver, 
                                      Message* _message, float _delay)
      : sender(_sender), receiver(_receiver), 
      message(_message), delay(_delay)
{
}

///////////////////////////////////////////////////////////////////////////////

MessageDispatcher::Telegram::Telegram(const Telegram& rhs)
      : sender(rhs.sender),
      receiver(rhs.receiver),
      message(rhs.message->clone()),
      delay(rhs.delay)
{
}

///////////////////////////////////////////////////////////////////////////////

MessageDispatcher::Telegram::~Telegram()
{
   delete message;
   message = NULL;

   delay = -1;
}

///////////////////////////////////////////////////////////////////////////////
