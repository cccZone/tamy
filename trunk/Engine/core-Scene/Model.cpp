#include "core-Scene\Model.h"
#include "core-Scene\Entity.h"
#include "core\Serializer.h"
#include "core-Scene\ModelView.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class NullProgressObserver : public Model::ProgressObserver
   {
   public:
      void setProgress(float percentage) {}
   };
} // anonymous

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

void Model::save(Serializer& serializer, ProgressObserver* observer)
{
   ProgressObserver* po = observer;
   if (po == NULL)
   {
      po = new NullProgressObserver();
   }

   unsigned int count = m_entities.size();
   serializer.saveInt(count);
   for (unsigned int i = 0; i < count; ++i)
   {
      Entity& entity = *(m_entities[i]);

      serializer.saveInt(entity.getClassHandle());
      entity.save(serializer);

      po->setProgress((float)i / (float)count);
   }

   delete po;
}

///////////////////////////////////////////////////////////////////////////////

void Model::load(Serializer& serializer, ProgressObserver* observer)
{
   ProgressObserver* po = observer;
   if (po == NULL)
   {
      po = new NullProgressObserver();
   }

   unsigned int entitiesCount = serializer.loadInt();

   for (unsigned int i = 0; i < entitiesCount; ++i)
   {
      int entityClassID = serializer.loadInt();
      Entity* entity = Entity::create(entityClassID);

      entity->load(serializer);

      add(entity);

      po->setProgress((float)i / (float)entitiesCount);
   }

   delete po;
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
      view.onEntityAdded(*(m_entities[i]));
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::detach(ModelView& view)
{
   Views::iterator it = std::find(m_views.begin(), m_views.end(), &view);
   if (it != m_views.end())
   {
      *it = NULL;
      ++m_viewsToRemoveCount;
   }

   // we may be trying to remove the view we just added
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
