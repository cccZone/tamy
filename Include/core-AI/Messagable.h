#pragma once

#include "core-AI\Message.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This interfafce marks a class of objects that can receive 
 * messages from the messaging system
 */
class Messagable
{
public:
   virtual ~Messagable() {}

   /**
    * This method will be called when the instance receives a message
    */
   virtual void receiveMessage(Messagable& sender, Message& message) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * The macro can be used to make a Messagable respond
 * to a certain type of message
 */
#define RECEIVE_MESSAGES_FOR(MessagableClass)                                          \
void receiveMessage(Messagable& sender, Message& message)                              \
{                                                                                      \
   TMessage<MessagableClass>* m = dynamic_cast<TMessage<MessagableClass>*> (&message); \
   MessagableClass* cThis = dynamic_cast<MessagableClass*> (this);                     \
   if ((m != NULL) && (cThis != NULL))                                                 \
   {                                                                                   \
      m->perform(*cThis, sender);                                                      \
   }                                                                                   \
}

///////////////////////////////////////////////////////////////////////////////