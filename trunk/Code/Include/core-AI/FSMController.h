/// @file   core-AI/FSMController.h
#ifndef _FSM_CONTROLLER_H
#define _FSM_CONTROLLER_H

#include "core-AI/FSMState.h"
#include "core-AppFlow/TimeDependent.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * This abstract controller establishes a framework for managing FSM logic.
 * Controller implementations should implement methods that allow to switch
 * between states - these methods will thus be exposed to the states
 * that operate with such controller implementation.
 */
template< typename Impl >
class FSMController : public TimeDependent
{
private:
   FSMState< Impl >*                   m_currentState;
   FSMState< Impl >*                   m_nextState;
   std::vector< FSMState< Impl >* >    m_states;

public:
   FSMController();
   virtual ~FSMController();

   /**
    * Returns the instance of currently active state.
    */
   template< typename StateInterface >
   StateInterface& getCurrentState() const;

   /**
    * Registers a new FSM state.
    */
   template< typename State >
   void registerState();

   /**
    * Registers a new FSM state.
    *
    * @return  instance of the state we'll be starting with
    */
   template< typename State >
   State& begin();

   /**
    * Returns an instance of the specified state.
    */
   template< typename State >
   State& getState();

   /**
    * Switches to a different state.
    *
    * @return  instance of the state we're switching to
    */
   template< typename State >
   State& transitionTo();

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-AI\FSMController.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _FSM_CONTROLLER_H
