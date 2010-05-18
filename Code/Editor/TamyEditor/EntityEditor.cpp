#include "EntityEditor.h"
#include "core-MVC.h"
#include "core.h"
#include <QMenu.h>


///////////////////////////////////////////////////////////////////////////////

EntityEditor::EntityEditor( const Filesystem& fs, Entity& editedEntity )
: m_iconsDir( fs.getShortcut( "editorIcons" ) )
, m_editedEntity( editedEntity )
{
}

///////////////////////////////////////////////////////////////////////////////

void EntityEditor::createAddEntityAction( QMenu& parent, const Class& entityClass )
{
   parent.addAction( new EEAddEntityAction( parent, m_editedEntity, entityClass, m_iconsDir ) );
}

///////////////////////////////////////////////////////////////////////////////

void EntityEditor::createRemoveEntityAction( QMenu& parent, Entity* entity )
{
   parent.addAction( new EERemoveEntityAction( parent, m_editedEntity, entity, m_iconsDir ) );
}

///////////////////////////////////////////////////////////////////////////////

void EntityEditor::createClearAction( QMenu& parent )
{
   parent.addAction( new EEClearAction( parent, m_editedEntity, m_iconsDir ) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

EEAddEntityAction::EEAddEntityAction( QWidget& parent, Entity& parentEntity, const Class& addedClass, const std::string& iconsDir )
: QAction( &parent )
, m_parentEntity( parentEntity )
, m_addedClass( addedClass )
{
   std::string shortClassName = addedClass.getShortName();
   setText( shortClassName.c_str() );
   setIcon( QIcon( ( iconsDir + shortClassName + "Icon.png" ).c_str() ) );
   connect( this, SIGNAL( triggered() ), this, SLOT( execute() ) );
}

///////////////////////////////////////////////////////////////////////////////

void EEAddEntityAction::execute()
{
   Entity* newEntity = m_addedClass.instantiate< Entity >();
   m_parentEntity.add( newEntity );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

EERemoveEntityAction::EERemoveEntityAction( QWidget& parent, Entity& editedEntity, Entity* entityToRemove, const std::string& iconsDir )
: QAction( &parent )
, m_editedEntity( editedEntity )
, m_entityToRemove( entityToRemove )
{
   setText( "Delete" );
   setIcon( QIcon( ( iconsDir + "quit.png" ).c_str() ) );
   connect( this, SIGNAL( triggered() ), this, SLOT( execute() ) );
}

///////////////////////////////////////////////////////////////////////////////


void EERemoveEntityAction::execute()
{
   m_editedEntity.remove( *m_entityToRemove );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

EEClearAction::EEClearAction( QWidget& parent, Entity& editedEntity, const std::string& iconsDir )
: QAction( &parent )
, m_editedEntity( editedEntity )
{
   setText( "Clear" );
   setIcon( QIcon( ( iconsDir + "clear.png" ).c_str() ) );
   connect( this, SIGNAL( triggered() ), this, SLOT( execute() ) );
}

///////////////////////////////////////////////////////////////////////////////

void EEClearAction::execute()
{
   std::vector< Entity* > children = m_editedEntity.getEntityChildren();
   for ( std::vector< Entity* >::iterator it = children.begin();
      it != children.end(); ++it )
   {
      m_editedEntity.remove( **it );
   }
}

///////////////////////////////////////////////////////////////////////////////
