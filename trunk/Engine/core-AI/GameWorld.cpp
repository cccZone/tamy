#include "core-AI\GameWorld.h"
#include "core-AI\GameWorldView.h"
#include "core-AI\Entity.h"
#include <stdexcept>
#include <cassert>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

GameWorld::GameWorld()
: m_view(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

GameWorld::~GameWorld()
{
   unsigned int count = m_entities.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_entities[i];
   }
   m_entities.clear();
}

///////////////////////////////////////////////////////////////////////////////

int GameWorld::getEntitiesCount() const
{
   return m_entities.size();
}

///////////////////////////////////////////////////////////////////////////////

void GameWorld::addEntity(Entity* newEntity)
{
   if (newEntity == NULL)
   {
      throw std::invalid_argument("NULL pointer instead an Entity instance");
   }
   m_entities.push_back(newEntity);

   if (m_view) {m_view->onEntityAdded(*newEntity);}
}

///////////////////////////////////////////////////////////////////////////////

void GameWorld::removeEntity(Entity& entity)
{
   if (m_view) {m_view->onEntityRemoved(entity);}

   EntitiesColl::iterator it = std::find(m_entities.begin(), m_entities.end(), &entity);
   if (it == m_entities.end()) 
   {
      throw std::invalid_argument("Trying to remove an unregisted entity");
   }

   delete *it;
   m_entities.erase(it);
}

///////////////////////////////////////////////////////////////////////////////

void GameWorld::attachObserver(GameWorldView& observer)
{
   m_view = &observer;
}

///////////////////////////////////////////////////////////////////////////////
