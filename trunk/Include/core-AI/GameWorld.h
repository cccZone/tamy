#pragma once

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Entity;
class Controller;
class GameWorldView;

///////////////////////////////////////////////////////////////////////////////

/**
 * Representation of the world the game takes place in.
 */
class GameWorld
{
private:
   typedef std::vector<Entity*>  EntitiesColl;
   EntitiesColl m_entities;
   GameWorldView* m_view;

public:
   GameWorld();
   ~GameWorld();

   /**
    * Informs how many entities there are in the game world.
    *
    * @return        total number of buildings
    */
   int getEntitiesCount() const;

   /**
    * Allows to attach an observer that will be informed about
    * changes in the world.
    * There can only be one observer attached - they won't be stacked.
    * Use a composite if you wnat to attach more.
    *
    * @param observer   observer we wish to attach
    */
   void attachObserver(GameWorldView& observer);

   /**
    * The method adds a new entity to the world.
    *
    * @param newEntity     entity we want to add (can't be NULL).
    */
   void addEntity(Entity* newEntity);

   /**
    * The method removes an entity from the world.
    *
    * @param entity     entity we want to remove
    */
   void removeEntity(Entity& entity);
};

///////////////////////////////////////////////////////////////////////////////
