#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * This FSM state contains logic behind a single state of mind of an agent.
 * It's designed to work with a specific type of controller only - thus
 * the controller(1)-(*)states group create a family of classes working
 * together.
 */
template<class TLogicStateCtrl>
class FsmLogicState
{
public:
   virtual ~FsmLogicState() {}

   /**
    * This method will be called once the state is activated.
    *
    * @param controller     controller managing the states family
    */
   virtual void onEnter(TLogicStateCtrl& controller) = 0;

   /**
    * This method will be called once the state is deactivated.
    *
    * @param controller     controller managing the states family
    */
   virtual void onExit(TLogicStateCtrl& controller) = 0;

   /**
    * This method will be called on an active state each time
    * the controller gets updated.
    *
    * @param controller     controller managing the states family
    * @param timeElapsed   time (in seconds) that has elapsed
    *                      since the last time this method was called
    */
   virtual void evaluate(TLogicStateCtrl& controller, float timeElapsed) = 0;
};

///////////////////////////////////////////////////////////////////////////////
