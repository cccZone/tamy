#include "core-Scene\Model.h"
#include "core-Scene\Entity.h"
#include "core-Scene\ModelSerializer.h"
#include "core-Scene\ModelView.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

Model::~Model() 
{
   unsigned int viewsCount = m_views.size();
   for (unsigned int i = 0; i < viewsCount; ++i)
   {
      m_views[i]->onDetachedFromModel();
   }
   m_views.clear();

   clear();
}

///////////////////////////////////////////////////////////////////////////////

void Model::add(Entity* entity)
{
   if (entity == NULL)
   {
      throw std::invalid_argument("NULL pointer instead an Entity instance");
   }

   m_entities.push_back(entity);
   notifyEntityAdded(*entity);
}

///////////////////////////////////////////////////////////////////////////////

void Model::remove(Entity& entity, bool release)
{
   Entities::iterator it = std::find(m_entities.begin(), m_entities.end(), &entity);
   if (it != m_entities.end())
   {
      notifyEntityRemoved(entity);
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
      notifyEntityRemoved(*(m_entities[i]));
      delete m_entities[i];
   }
   m_entities.clear();
}

///////////////////////////////////////////////////////////////////////////////

void Model::save(ModelSerializer& modelSerializer)
{
   unsigned int count = m_entities.size();
   modelSerializer.saveInt(count);
   for (unsigned int i = 0; i < count; ++i)
   {
      Entity& entity = *(m_entities[i]);

      modelSerializer.saveInt(entity.getClassHandle());
      entity.save(modelSerializer);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::load(ModelSerializer& modelSerializer)
{
   unsigned int entitiesCount = modelSerializer.loadInt();

   for (unsigned int i = 0; i < entitiesCount; ++i)
   {
      int entityClassID = modelSerializer.loadInt();
      Entity* entity = Entity::create(entityClassID);

      entity->load(modelSerializer);

      add(entity);
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
   m_views.push_back(&view);
   view.onAttachedToModel(*this);

   // notify the view about all entities
   unsigned int entitiesCount = m_entities.size();
   for (unsigned int i = 0; i < entitiesCount; ++i)
   {
      view.onEntityAdded(*(m_entities[i]));
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::detach(ModelView& view)
{
   Views::iterator it = std::find(m_views.begin(), m_views.end(), &view);
   if (it != m_views.end())
   {
      m_views.erase(it);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::notifyEntityAdded(Entity& entity)
{
   unsigned int count = m_views.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_views[i]->onEntityAdded(entity);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::notifyEntityRemoved(Entity& entity)
{
   unsigned int count = m_views.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_views[i]->onEntityRemoved(entity);
   }
}

///////////////////////////////////////////////////////////////////////////////

unsigned int Model::getViewsCount() const
{
   return m_views.size();
}

///////////////////////////////////////////////////////////////////////////////
