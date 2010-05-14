#include "core-MVC\Entity.h"
#include "core-MVC\Model.h"
#include <stdexcept>
#include <typeinfo>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT(Entity, Object)
   PROPERTY("parent", Entity*, m_parent)
   PROPERTY("children", Entity::Children, m_children)
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Entity::Entity()
: m_parent(NULL)
, m_properties(NULL)
, m_hostModel(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

Entity::~Entity()
{
   m_parent = NULL;

   unsigned int count = m_children.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_children[i];
   }
   m_children.clear();
}

///////////////////////////////////////////////////////////////////////////////

void Entity::add(Entity* entity)
{
   if (entity == NULL)
   {
      throw std::invalid_argument("NULL pointer instead an Entity instance");
   }

   // reparent the entity
   if (entity->m_parent != NULL)
   {
      entity->m_parent->remove(*entity, false);
   }
   entity->m_parent = this;

   // register it as a child
   m_children.push_back(entity);

   if (m_hostModel != NULL)
   {
      m_hostModel->add(entity, false);
   }

   // send proper notifications
   onChildAttached(*entity);
   entity->onAttached(*this);
}

///////////////////////////////////////////////////////////////////////////////

void Entity::remove(Entity& entity, bool release)
{
   // send proper notifications
   onChildDetached(entity);
   entity.onDetached(*this);
   entity.m_parent = NULL;

   // remove the record of the entity from the children collections
   // and from all affiliated models
   if (m_hostModel != NULL)
   {
      m_hostModel->remove(entity);
   }

   Children::iterator it = std::find(m_children.begin(), m_children.end(), &entity);
   if (it != m_children.end())
   {
      m_children.erase(it);

      if (release == true)
      {
         delete &entity;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void Entity::onAttachToModel(Model& model)
{
   if (m_hostModel != NULL)
   {
      throw std::runtime_error("This entity is already attached to a model");
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
      throw std::runtime_error("The model this entity is being detached from is not its host");
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
