#include "core-MVC\Entity.h"
#include "core-MVC\Model.h"
#include "core\Assert.h"
#include <typeinfo>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( Entity )
   PARENT( ReflectionObject );
   PROPERTY_EDIT( "name", std::string, m_name )
   PROPERTY( Entity*, m_parent )
   PROPERTY( Entity::Children, m_managedChildren )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Entity::Entity( const std::string& name )
   : m_name( name )
   , m_parent( NULL )
   , m_hostModel( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

Entity::Entity( const Entity& rhs )
   : m_name( rhs.m_name )
   , m_parent( NULL )
   , m_hostModel( NULL )
{

}

///////////////////////////////////////////////////////////////////////////////

Entity::~Entity()
{
   m_parent = NULL;

   unsigned int count = m_managedChildren.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_managedChildren[i];
   }
   m_managedChildren.clear();
   m_children.clear();
}

///////////////////////////////////////////////////////////////////////////////

Entity* Entity::clone() const
{
   // clone self
   Entity* selfClone = cloneSelf();

   // clone the children and attach them
   unsigned int count = m_children.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      Entity* childClone = m_children[i]->clone();
      selfClone->add( childClone );
   }

   return selfClone;
}

///////////////////////////////////////////////////////////////////////////////

void Entity::add( Entity* entity, bool manage )
{
   if ( entity == NULL )
   {
      ASSERT_MSG( false, "NULL pointer instead an Entity instance" );
      return;
   }

   // reparent the entity
   if ( entity->m_parent != NULL )
   {
      entity->m_parent->remove( *entity, false );
   }
   entity->m_parent = this;

   // register it as a child
   if ( manage )
   {
      m_managedChildren.push_back( entity );
   }
   m_children.push_back( entity );

   if ( m_hostModel != NULL )
   {
      m_hostModel->add( entity, false );
   }

   // send proper notifications
   onChildAttached( *entity );
   entity->onAttached( *this );
}

///////////////////////////////////////////////////////////////////////////////

void Entity::remove( Entity& entity, bool release )
{
   // send proper notifications
   onChildDetached( entity );
   entity.onDetached( *this );

   // remove the record of the entity from the children collections
   // and from all affiliated models
   if ( m_hostModel != NULL )
   {
      m_hostModel->remove( entity );
   }
   entity.m_parent = NULL;

   // remove the entity from the managed entities list
   Children::iterator it = std::find( m_managedChildren.begin(), m_managedChildren.end(), &entity );
   if ( it != m_managedChildren.end() )
   {
      m_managedChildren.erase( it );

      if ( release == true )
      {
         delete &entity;
      }
   }

   // remove the entity from the runtime entities list
   it = std::find( m_children.begin(), m_children.end(), &entity );
   if ( it != m_children.end() )
   {
      m_children.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Entity::update( float timeElapsed )
{
   // update the state of the entity
   onUpdate( timeElapsed );

   // pass the request down the tree
   for ( Children::iterator it = m_children.begin(); it != m_children.end(); ++it )
   {
      (*it)->update( timeElapsed );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Entity::onAttachToModel(Model& model)
{
   if (m_hostModel != NULL)
   {
      ASSERT_MSG( false, "This entity is already attached to a model");
   }

   m_hostModel = &model;
   onAttached(model);

   unsigned int count = m_children.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_children[i]->onAttachToModel(model);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Entity::onDetachFromModel(Model& model)
{
   if (&model != m_hostModel)
   {
      ASSERT_MSG( false,"The model this entity is being detached from is not its host");
   }

   unsigned int count = m_children.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_children[i]->onDetachFromModel(model);
   }

   onDetached(model);
   m_hostModel = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Entity::onObjectLoaded()
{
   __super::onObjectLoaded();

   if ( m_parent )
   {
      onAttached( *m_parent );
   }

   // copy the managed children to the runtime children array
   unsigned int count = m_managedChildren.size();
   m_children.resize( count );
   for( unsigned int i = 0; i < count; ++i )
   {
      m_children[i] = m_managedChildren[i];
   }

   // inform that the children have been attached
   Children children = m_children;
   for ( Children::iterator it = children.begin(); it != children.end(); ++it )
   {
      onChildAttached( **it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Entity::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( m_hostModel )
   {
      m_hostModel->notifyEntityChanged( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////
