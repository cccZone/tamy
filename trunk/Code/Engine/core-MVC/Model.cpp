#include "core-MVC\Model.h"
#include "core-MVC\Entity.h"
#include "core-MVC\ModelView.h"
#include "core-MVC\ModelComponent.h"
#include "core\Stack.h"
#include "core\Assert.h"
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

BEGIN_RESOURCE( Model, tsc, AM_BINARY );
   PROPERTY( Entities, m_managedEntities );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

Model::Model( const FilePath& resourceName )
   : Resource( resourceName )
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
   if ( entity == NULL )
   {
      ASSERT_MSG( false, "NULL pointer instead an Entity instance" );
      return;
   }

   if ( manage == true )
   {
      m_managedEntities.push_back( entity );
   }

   m_entities.push_back( entity );
   entity->onAttachToModel(*this);
   entityDFS( *entity, Functor::FROM_METHOD( Model, notifyEntityAdded, this ) );

   // inform the entity about the registered components
   unsigned int count = getComponentsCount();
   for ( unsigned int i = 0; i < count; ++i )
   {
      ComponentAddOperation op( *this, *getComponent( i ) );
      entityDFS( *entity, Functor::FROM_METHOD( ComponentAddOperation, notify, &op ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::remove( Entity& entity )
{
   entityDFS( entity, Functor::FROM_METHOD( Model, notifyEntityRemoved, this ) );
   entity.onDetachFromModel( *this );

   // remove it from the managed entities list ( and delete it )
   Entities::iterator it = std::find( m_managedEntities.begin(), m_managedEntities.end(), &entity );
   if ( it != m_managedEntities.end() )
   {
      m_managedEntities.erase( it );
      delete &entity;
   }

   // remove it from the entities list
   it = std::find( m_entities.begin(), m_entities.end(), &entity );
   if ( it != m_entities.end() )
   {
      m_entities.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::clear()
{
   processViewsOperations();

   // inform the attached entities about the components being removed from them
   unsigned int componentsCount = getComponentsCount();
   for ( unsigned int compIdx = 0; compIdx < componentsCount; ++compIdx )
   {
      Component< Model >* comp = getComponent( compIdx );
      onComponentRemoved( *comp );
   }

   // delete the managed entities
   unsigned int count = m_managedEntities.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_managedEntities[i];
   }
   m_managedEntities.clear();

   // and remove all added entities
   m_entities.clear();

   // clear the views
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

void Model::clone( std::vector< Entity* >& outClonedEntities ) const
{
   unsigned int count = m_entities.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      Entity* clonedEntity = m_entities[i]->clone();
      if ( clonedEntity != NULL )
      {
         outClonedEntities.push_back( clonedEntity );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::update( float timeElapsed )
{
   // update the state of the components
   ComponentsManager< Model >::update( timeElapsed );

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

Entity& Model::getEntity( unsigned int idx ) const
{
   return *m_entities.at( idx );
}

///////////////////////////////////////////////////////////////////////////////

Entity* Model::findFirstEntity( const std::string& name ) const
{
   // notify the view about all entities
   unsigned int entitiesCount = m_entities.size();
   for (unsigned int i = 0; i < entitiesCount; ++i)
   {
      Stack< Entity* > entities;
      entities.push( m_entities[i] );

      while ( entities.empty() == false )
      {
         Entity* currEntity = entities.pop();
         if ( currEntity->getEntityName() == name )
         {
            return currEntity;
         }

         const Entity::Children& children = currEntity->getEntityChildren();
         unsigned int count = children.size();
         for ( unsigned int i = 0; i < count; ++i )
         {
            entities.push( children[i] );
         }
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Model::attach( ModelView& view )
{
   m_viewsToAdd.push_back(&view);
   view.onAttachedToModel(*this);

   // notify the view about all entities
   unsigned int entitiesCount = m_entities.size();
   for (unsigned int i = 0; i < entitiesCount; ++i)
   {
      entityDFS( *m_entities[i], Functor::FROM_METHOD( ModelView, onEntityAdded, &view ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::detach( ModelView& view )
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
   for ( Views::iterator it = m_views.begin(); ( it != m_views.end() ) && ( m_viewsToRemoveCount > 0 ); )
   {
      if ( *it == NULL )
      {
         it = m_views.erase( it );
         --m_viewsToRemoveCount;
      }
      else
      {
         ++it;
      }
   }

   // add new views
   if ( m_viewsToAdd.empty() == false )
   {
      m_views.insert( m_views.end(), m_viewsToAdd.begin(), m_viewsToAdd.end() );
      m_viewsToAdd.clear();
   }
}

///////////////////////////////////////////////////////////////////////////////

void Model::notifyEntityAdded( Entity& entity )
{
   processViewsOperations();

   unsigned int count = m_views.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_views[i] != NULL )
      {
         m_views[i]->onEntityAdded( entity );
      }
   }

   processViewsOperations();
}

///////////////////////////////////////////////////////////////////////////////

void Model::notifyEntityRemoved( Entity& entity )
{
   processViewsOperations();
   
   unsigned int count = m_views.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_views[i] != NULL )
      {
         m_views[i]->onEntityRemoved( entity );
      }
   }

   processViewsOperations();
}

///////////////////////////////////////////////////////////////////////////////

void Model::notifyEntityChanged( Entity& entity )
{
   processViewsOperations();

   unsigned int count = m_views.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_views[i] != NULL )
      {
         m_views[i]->onEntityChanged( entity );
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

void Model::entityDFS( Entity& entity, const Functor& operation )
{
   Stack< Entity* > entities;
   entities.push( &entity );

   while ( entities.empty() == false )
   {
      Entity* currEntity = entities.pop();
      operation( *currEntity );

      const Entity::Children& children = currEntity->getEntityChildren();
      unsigned int count = children.size();
      for ( unsigned int i = 0; i < count; ++i )
      {
         entities.push( children[i] );
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

   // copy all managed entities to the runtime entities list
   unsigned int count = m_managedEntities.size();
   m_entities.resize( count );
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_entities[i] = m_managedEntities[i];
   }

   // inform all entities that they are loaded
   for ( Entities::iterator it = m_entities.begin(); it != m_entities.end(); ++it )
   {
      (*it)->onAttachToModel(*this);
   }
}

///////////////////////////////////////////////////////////////////////////////
