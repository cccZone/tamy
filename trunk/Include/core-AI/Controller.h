#pragma once


///////////////////////////////////////////////////////////////////////////////

class BulletPhysics;

///////////////////////////////////////////////////////////////////////////////

/**
 * Purpose of this interface is to mark classes that can be propelled
 * by the game world.
 */
class Controller
{
public:
   virtual ~Controller() {}

   /**
    * This method should update the state of whatever the controller
    * is controlling.
    * 
    * @param timeElapsed   amount time (in seconds) that has elapsed since
    *                      the last time this method was called
    */
   virtual void update(float timeElapsed) = 0;
};

///////////////////////////////////////////////////////////////////////////////
