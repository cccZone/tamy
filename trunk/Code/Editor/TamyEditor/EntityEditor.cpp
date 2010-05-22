#include "EntityEditor.h"
#include "core-MVC.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

EntityEditor::EntityEditor( Entity& editedEntity )
: m_editedEntity( editedEntity )
{
}

///////////////////////////////////////////////////////////////////////////////

void EntityEditor::addEntity( const Class& entityClass )
{
   Entity* newEntity = entityClass.instantiate< Entity >();
   m_editedEntity.add( newEntity );
}

///////////////////////////////////////////////////////////////////////////////

void EntityEditor::removeEntity( Entity* entity )
{
   m_editedEntity.remove( *entity );
}

///////////////////////////////////////////////////////////////////////////////

void EntityEditor::clearEntity()
{
   std::vector< Entity* > children = m_editedEntity.getEntityChildren();
   for ( std::vector< Entity* >::iterator it = children.begin();
      it != children.end(); ++it )
   {
      m_editedEntity.remove( **it );
   }
}

///////////////////////////////////////////////////////////////////////////////
