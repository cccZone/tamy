#pragma once


///////////////////////////////////////////////////////////////////////////////

class Entity;

///////////////////////////////////////////////////////////////////////////////

/**
 * Using this interface we can watch the situation in the game world.
 */
class GameWorldView
{
public:
   virtual ~GameWorldView() {}

   /**
    * The method is called when a new entity appears in the game world.
    *
    * @param entity     entity that's been added
    */
   virtual void onEntityAdded(Entity& entity) = 0;

   /**
    * The method is called when an entity is about to be removed 
    * from the game world.
    *
    * @param entity     entity that's gonna be removed
    */
   virtual void onEntityRemoved(Entity& entity) = 0;
};

///////////////////////////////////////////////////////////////////////////////
