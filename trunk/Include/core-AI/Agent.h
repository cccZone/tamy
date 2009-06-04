#pragma once

#include "core-AI\AgentNode.h"
#include "core-AI\StateMachine.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * An agent represents an interactive thing in the game world.
 * It has a state machine that manages its actions
 */
template<class T>
class Agent : public AgentNode
{
private:
   StateMachine<T>* m_fsm;

public:
   virtual ~Agent() 
   {
      delete m_fsm;
      m_fsm = NULL;
   }

   RECEIVE_MESSAGES_FOR(T);

   void update()
   {
      m_fsm->update();
   }

protected:
   Agent(const std::string& name, bool dynamic) 
         : AgentNode(name, dynamic), m_fsm(new StateMachine<T>())
   {}

   inline StateMachine<T>& fsm() {return *m_fsm;}
};

///////////////////////////////////////////////////////////////////////////////
