#pragma once

#include <list>


///////////////////////////////////////////////////////////////////////////////

class Messagable;
class Message;

///////////////////////////////////////////////////////////////////////////////

class MessageDispatcher
{
private:
   struct Telegram
   {
      Messagable& sender;
      Messagable& receiver;
      Message* message;
      float delay;

      Telegram(Messagable& _sender, 
              Messagable& _receiver, 
              Message* _message, float _delay);
      Telegram(const Telegram& rhs);
      ~Telegram();
   };

   typedef std::list<Telegram> MessagesQueue;

private:
   MessagesQueue m_msgQueue;

public:
   void send(Messagable& sender, 
             Messagable& receiver, 
             Message* message, float delay);

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////
