/// @file   core-AI\MessageDispatcher.h
/// @brief  dispatches messages between AI agents
#pragma once

#include <list>
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

class Messagable;
class Message;

///////////////////////////////////////////////////////////////////////////////

class MessageDispatcher
{
   DECLARE_ALLOCATOR( MessageDispatcher, AM_DEFAULT );

private:
   struct Telegram
   {
      DECLARE_ALLOCATOR( Telegram, AM_DEFAULT );

      Messagable& sender;
      Messagable& receiver;
      Message* message;
      float delay;

      Telegram( Messagable& _sender, Messagable& _receiver, Message* _message, float _delay );
      Telegram( const Telegram& rhs );
      ~Telegram();
   };

   typedef std::list<Telegram> MessagesQueue;

private:
   MessagesQueue m_msgQueue;

public:
   void send( Messagable& sender, Messagable& receiver, Message* message, float delay );

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////
