#include "core-MVC\Model.h"
#include "core-MVC\Entity.h"
#include "core-MVC\ModelView.h"
#include "core\Serializer.h"
#include "core\Stack.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( Model, tsc, AM_BINARY )
   PROPERTY( "entities", Entities, m_entities )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

Model::Model()
: m_viewsToRemoveCount(0)
{
}

///////////////////////////////////////////////////////////////////////////////

Model::~Model() 
{
   unsigned int count = m_views.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if (m_views[i] != NULL)
      {
         m_views[i]->onDetachedFromModel();
      }
   }
   m_views.clear();

   count = m_viewsToAdd.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_viewsToAdd[i]->onDetachedFromModel();
   }
   m_viewsToAdd.clear();

   clear();
}

///////////////////////////////////////////////////////////////////////////////

void Model::add(Entity* entity, bool manage)
{
   if (entity == NULL)
   {
      throw std::invalid_argument("NULL pointer instead an Entity instance");
   }

   if (manage == true)
   {
      m_entities.push_back(entity);
   }
   entity->onAttachToModel(*this);
   entityDFS(*entity, Functor::FROM_METHOD(Model, notifyEntityAdded, this));
}

///////////////////////////////////////////////////////////////////////////////

void Model::remove(Entity& entity, bool release)
{
   entity.onDetachFromModel(*this);
   entityDFS(entity, Functor::FROM_METHOD(Model, notifyEntityRemoved, this));

   Entities::iterator it = std::find(m_entities.begin(), m_entities.end(), &entity);
   if (it != m_entities.end())
   {
      m_entities.erase(it);

      if (release == true)
      {
         delete &entity;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::clear()
{
   unsigned int count = m_entities.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_entities[i];
   }
   m_entities.clear();

   count = m_views.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if (m_views[i] != NULL)
      {
         m_views[i]->resetContents();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

unsigned int Model::getEntitiesCount() const
{
   return m_entities.size();
}

///////////////////////////////////////////////////////////////////////////////

Entity& Model::getEntity(unsigned int idx)
{
   return *(m_entities.at(idx));
}

///////////////////////////////////////////////////////////////////////////////

void Model::attach(ModelView& view)
{
   m_viewsToAdd.push_back(&view);
   view.onAttachedToModel(*this);

   // notify the view about all entities
   unsigned int entitiesCount = m_entities.size();
   for (unsigned int i = 0; i < entitiesCount; ++i)
   {
      entityDFS(*(m_entities[i]), Functor::FROM_METHOD(ModelView, onEntityAdded, &view));
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::detach(ModelView& view)
{
   view.onDetachedFromModel();

   Views::iterator it = std::find(m_views.begin(), m_views.end(), &view);
   if (it != m_views.end())
   {
      *it = NULL;
      ++m_viewsToRemoveCount;
   }

   // we may be trying to remove a view we've just added
   it = std::find(m_viewsToAdd.begin(), m_viewsToAdd.end(), &view);
   if (it != m_viewsToAdd.end())
   {
      m_viewsToAdd.erase(it);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::processViewsOperations()
{
   // remove obsolete view
   for (Views::iterator it = m_views.begin();
        (it != m_views.end()) && (m_viewsToRemoveCount > 0);)
   {
      if (*it == NULL)
      {
         it = m_views.erase(it);
         --m_viewsToRemoveCount;
      }
      else
      {
         ++it;
      }
   }

   // add new views
   if (m_viewsToAdd.empty() == false)
   {
      m_views.insert(m_views.end(), m_viewsToAdd.begin(), m_viewsToAdd.end());
      m_viewsToAdd.clear();
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::notifyEntityAdded(Entity& entity)
{
   processViewsOperations();

   unsigned int count = m_views.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if (m_views[i] != NULL)
      {
         m_views[i]->onEntityAdded(entity);
      }
   }

   processViewsOperations();
}

///////////////////////////////////////////////////////////////////////////////

void Model::notifyEntityRemoved(Entity& entity)
{
   processViewsOperations();
   
   unsigned int count = m_views.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if (m_views[i] != NULL)
      {
         m_views[i]->onEntityRemoved(entity);
      }
   }

   processViewsOperations();
}

///////////////////////////////////////////////////////////////////////////////

unsigned int Model::getViewsCount() const
{
   return m_views.size() + m_viewsToAdd.size() - m_viewsToRemoveCount;
}

///////////////////////////////////////////////////////////////////////////////

void Model::entityDFS(Entity& entity, const Functor& operation)
{
   Stack<Entity*> entities;
   entities.push(&entity);

   while (entities.empty() == false)
   {
      Entity* currEntity = entities.pop();
      operation(*currEntity);

      const Entity::Children& children = currEntity->getEntityChildren();
      unsigned int count = children.size();

      for (unsigned int i = 0; i < count; ++i)
      {
         entities.push(children[i]);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
