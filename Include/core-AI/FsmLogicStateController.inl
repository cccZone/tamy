#ifndef _FSM_LOGIC_STATE_CONTROLLER_H
#error "This file can only be included from FsmLogicStateController.h"
#else

#include <string>


///////////////////////////////////////////////////////////////////////////////

template<class Impl>
FsmLogicStateController<Impl>::FsmLogicStateController() 
: m_currentState(NULL)
{}

///////////////////////////////////////////////////////////////////////////////

template<class Impl>
FsmLogicStateController<Impl>::~FsmLogicStateController()
{
}

///////////////////////////////////////////////////////////////////////////////

template<class Impl>
void FsmLogicStateController<Impl>::update(float timeElapsed)
{
   if (m_currentState != NULL) 
   {
      m_currentState->evaluate(dynamic_cast<Impl&> (*this), timeElapsed);
   }
}

///////////////////////////////////////////////////////////////////////////////

template<class Impl>
void FsmLogicStateController<Impl>::changeState(FsmLogicState<Impl>& newState)
{
   Impl& controller = dynamic_cast<Impl&> (*this);
   if (m_currentState) {m_currentState->onExit(controller);}
   m_currentState = &newState;
   m_currentState->onEnter(controller);
}

///////////////////////////////////////////////////////////////////////////////

#endif // _FSM_LOGIC_STATE_CONTROLLER_H
