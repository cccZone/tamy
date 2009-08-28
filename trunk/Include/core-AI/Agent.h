#pragma once

#include "core-AI\StateMachine.h"
#include "core-AI\Messagable.h"
#include "core-AI\MessageDispatcher.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * An agent represents an interactive thing in the game world.
 * It has a state machine that manages its actions.
 */
template<class T>
class Agent : public Messagable
{
private:
   std::string m_name;
   StateMachine<T>* m_fsm;
   MessageDispatcher* m_dispatcher;

public:
   virtual ~Agent() 
   {
      delete m_fsm;
      m_fsm = NULL;
   }

   RECEIVE_MESSAGES_FOR(T);

   const std::string& getName() const {return m_name;}

   void update()
   {
      m_fsm->update();
   }

   void setMessageDispatcher(MessageDispatcher& dispatcher)
   {
      m_dispatcher = &dispatcher;
   }

protected:
   Agent(const std::string& name, bool dynamic) 
         : m_name(name), m_fsm(new StateMachine<T>())
         , m_dispatcher(NULL)
   {}

   inline StateMachine<T>& fsm() {return *m_fsm;}

   inline MessageDispatcher& dispatcher() {return *m_dispatcher;}
};

///////////////////////////////////////////////////////////////////////////////
