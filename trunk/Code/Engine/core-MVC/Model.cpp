#include "core-MVC\Model.h"
#include "core-MVC\Entity.h"
#include "core-MVC\ModelView.h"
#include "core-MVC\ModelComponent.h"
#include "core\Serializer.h"
#include "core\Stack.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

   /**
    * This operation allows to apply Model::entityDFS method when
    * calling the Entity's parametrized method 'notifyComponentAdded',
    * (which requires a Component< Model >& param)
    */
   class ComponentAddOperation
   {
   private:
      Model&               m_hostModel;
      Component< Model >&  m_component;

   public:
      ComponentAddOperation( Model& hostModel, Component< Model >& component )
         : m_hostModel( hostModel )
         , m_component( component )
      {}

      void notify( Entity& entity )
      {
         m_hostModel.notifyComponentAdded( entity, m_component );
      }
   };

   // -------------------------------------------------------------------------

   /**
    * This operation allows to apply Model::entityDFS method when
    * calling the Entity's parametrized method 'notifyComponentAdded',
    * (which requires a Component< Model >& param)
    */
   class ComponentRemoveOperation
   {
   private:
      Model&               m_hostModel;
      Component< Model >&  m_component;

   public:
      ComponentRemoveOperation( Model& hostModel, Component< Model >& component )
         : m_hostModel( hostModel )
         , m_component( component )
      {}

      void notify( Entity& entity )
      {
         m_hostModel.notifyComponentRemoved( entity, m_component );
      }
   };

///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( Model, Resource, tsc, AM_BINARY )
   PROPERTY( Entities, m_entities )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

Model::Model( const std::string& name )
: Resource( name )
, m_viewsToRemoveCount(0)
{
}

///////////////////////////////////////////////////////////////////////////////

Model::~Model() 
{
   // detach the views
   unsigned int count = m_views.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if (m_views[i] != NULL)
      {
         m_views[i]->onDetachedFromModel( *this );
      }
   }
   m_views.clear();

   count = m_viewsToAdd.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_viewsToAdd[i]->onDetachedFromModel( *this );
   }
   m_viewsToAdd.clear();

   clear();
}

///////////////////////////////////////////////////////////////////////////////

void Model::add( Entity* entity, bool manage )
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

   // inform the entity about the registered components
   unsigned int count = getComponentsCount();
   for ( unsigned int i = 0; i < count; ++i )
   {
      ComponentAddOperation op( *this, *getComponent( i ) );
      entityDFS( *entity, Functor::FROM_METHOD(ComponentAddOperation, notify, &op) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::remove( Entity& entity )
{
   entityDFS(entity, Functor::FROM_METHOD(Model, notifyEntityRemoved, this));
   entity.onDetachFromModel(*this);

   Entities::iterator it = std::find(m_entities.begin(), m_entities.end(), &entity);
   if (it != m_entities.end())
   {
      m_entities.erase(it);
      delete &entity;
   }

   it = std::find(m_entities.begin(), m_entities.end(), &entity);
   if (it != m_entities.end())
   {
      m_entities.erase(it);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::clear()
{
   processViewsOperations();

   unsigned int componentsCount = getComponentsCount();
   unsigned int count = m_entities.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if ( m_entities[i] )
      {
         // inform the entity about the components being removed
         for ( unsigned int compIdx = 0; compIdx < componentsCount; ++compIdx )
         {
            Component< Model >* comp = getComponent( compIdx );
            m_entities[i]->onComponentRemoved( *comp );
         }
      }
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

void Model::update( float timeElapsed )
{
   // update the state of the components
   __super::update( timeElapsed );

   // update the state of the entities
   unsigned int count = m_entities.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_entities[i]->update( timeElapsed );
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
   view.onDetachedFromModel( *this );

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

void Model::notifyEntityChanged( Entity& entity )
{
   processViewsOperations();

   unsigned int count = m_views.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if (m_views[i] != NULL)
      {
         m_views[i]->onEntityChanged(entity);
      }
   }

   processViewsOperations();
}

///////////////////////////////////////////////////////////////////////////////

void Model::notifyComponentAdded( Entity& entity, Component< Model >& component )
{
   entity.onComponentAdded( component );
}

///////////////////////////////////////////////////////////////////////////////

void Model::notifyComponentRemoved( Entity& entity, Component< Model >& component )
{
   entity.onComponentRemoved( component );
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

void Model::onComponentAdded( Component< Model >& component )
{
   ComponentAddOperation op( *this, component );

   // inform all attached entities about the component
   unsigned int count = m_entities.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      entityDFS( *m_entities[i], Functor::FROM_METHOD(ComponentAddOperation, notify, &op) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::onComponentRemoved( Component< Model >& component )
{
   ComponentRemoveOperation op( *this, component );

   // inform all attached entities about the component
   unsigned int count = m_entities.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      entityDFS( *m_entities[i], Functor::FROM_METHOD(ComponentRemoveOperation, notify, &op) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::onResourceLoaded( ResourcesManager& mgr )
{
   addComponent( new ModelComponent< ResourcesManager >( mgr ) );
}

///////////////////////////////////////////////////////////////////////////////

void Model::onObjectLoaded()
{
   __super::onObjectLoaded();

   // inform all entities that they are loaded
   for ( Entities::iterator it = m_entities.begin(); it != m_entities.end(); ++it )
   {
      (*it)->onAttachToModel(*this);
   }
}

///////////////////////////////////////////////////////////////////////////////
