#pragma once

#include "core-AI\State.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a Finite State Machine
 */
template<typename T>
class StateMachine
{
private:
   State<T>* m_currState;
public:
   StateMachine()
      : m_currState(NULL)
   {
   }

   virtual ~StateMachine() 
   {
      m_currState = NULL;
   }

   /**
    * This method activates a new state, deactivating the old one
    * if one exists
    */
   void setState(State<T>& state)
   {
      if (m_currState != NULL) {m_currState->onExit();}
      m_currState = & state;
      m_currState->onEnter();
   }

   /**
    * This method updates the currently active state
    */
   void update()
   {
      if (m_currState != NULL) {m_currState->onUpdate();}
   }
};

///////////////////////////////////////////////////////////////////////////////
