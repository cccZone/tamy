#pragma once

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Controller;

///////////////////////////////////////////////////////////////////////////////

class ControllersManager
{
private:
   typedef std::vector<Controller*> ControllersColl;
   ControllersColl m_controllers; 

   typedef std::vector<unsigned int> ControllersForRemoval;
   ControllersForRemoval m_toBeRemoved;

public:
   ~ControllersManager();

   /**
    * The method adds a new controller to the world.
    *
    * @param newController     controller we want to add (can't be NULL).
    */
   void addController(Controller* newController);

   /**
    * The method removes a controller from the world.
    *
    * @param controller     controller we want to remove
    */
   void removeController(Controller& controller);

   /**
    * The method advances the time of the game.
    *
    * @param timeElapsed   amount of time elapsed (in seconds)
    *                      since the last time this method was called
    */
   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////
