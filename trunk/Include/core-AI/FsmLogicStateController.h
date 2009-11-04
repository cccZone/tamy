#ifndef _FSM_LOGIC_STATE_CONTROLLER_H
#define _FSM_LOGIC_STATE_CONTROLLER_H

#include "core-AI\FsmLogicState.h"
#include "core-AI\Controller.h"



///////////////////////////////////////////////////////////////////////////////

/**
 * This abstract controller establishes a framework for managing FSM logic.
 * Controller implementations should implement methods that allow to switch
 * between states - these methods will thus be exposed to the states
 * that operate with such controller implementation.
 */
template<class Impl>
class FsmLogicStateController : public Controller
{
private:
   FsmLogicState<Impl>* m_currentState;

public:
   FsmLogicStateController();
   virtual ~FsmLogicStateController();

   /**
    * Executes the currently set fsm behavior.
    *
    * @param timeElapsed      time (in seconds) that has elapsed since
    *                         the last time this method was called
    */
   void update(float timeElapsed);

   /**
    * A utility method for switching between states.
    */
   void changeState(FsmLogicState<Impl>& newState);
};

///////////////////////////////////////////////////////////////////////////////

#include "core-AI\FsmLogicStateController.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _FSM_LOGIC_STATE_CONTROLLER_H
