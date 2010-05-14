#ifndef _ENTITY_H
#error "This file can only be included in Entity.h"
#else

#include "core-MVC\Model.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

const Entity::Children& Entity::getEntityChildren() const {return m_children;}

///////////////////////////////////////////////////////////////////////////////

bool Entity::isAttached() const { return m_parent != NULL; }

///////////////////////////////////////////////////////////////////////////////

Entity& Entity::getParent()
{
   ASSERT( m_parent != NULL, "Entity doesn't have a parent" );
   return *m_parent;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _ENTITY_H
