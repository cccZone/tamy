#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * A state represents a state of an FSM, in which an agent performs
 * some specific actions
 */
template<typename T>
class State
{
protected:
   T& m_agent;

public:
   virtual ~State() {}

   /**
    * This method is called once the state is activated
    */
   virtual void onEnter() = 0;

   /**
    * This method is called when it's active and the state machine
    * is updated
    */
   virtual void onUpdate() = 0;

   /**
    * This method is called upon state deactivation
    */
   virtual void onExit() = 0;

protected:
   State(T& agent) : m_agent(agent) {}
};

///////////////////////////////////////////////////////////////////////////////
