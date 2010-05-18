#include "ModelEditor.h"
#include "core-MVC.h"
#include "core.h"
#include <QMenu.h>


///////////////////////////////////////////////////////////////////////////////

ModelEditor::ModelEditor( const Filesystem& fs, Model& scene )
: m_iconsDir( fs.getShortcut( "editorIcons" ) )
, m_scene( scene )
{
}

///////////////////////////////////////////////////////////////////////////////

void ModelEditor::createAddEntityAction( QMenu& parent, const Class& entityClass )
{
   parent.addAction( new MEAddEntityAction( parent, m_scene, entityClass, m_iconsDir ) );
}

///////////////////////////////////////////////////////////////////////////////

void ModelEditor::createRemoveEntityAction( QMenu& parent, Entity* entity )
{
   parent.addAction( new MERemoveEntityAction( parent, m_scene, entity, m_iconsDir ) );
}

///////////////////////////////////////////////////////////////////////////////

void ModelEditor::createClearAction( QMenu& parent )
{
   parent.addAction( new MEClearAction( parent, m_scene, m_iconsDir ) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

MEAddEntityAction::MEAddEntityAction( QWidget& parent, Model& scene, const Class& addedClass, const std::string& iconsDir )
: QAction( &parent )
, m_scene( scene )
, m_addedClass( addedClass )
{
   std::string shortClassName = addedClass.getShortName();
   setText( shortClassName.c_str() );
   setIcon( QIcon( ( iconsDir + shortClassName + "Icon.png" ).c_str() ) );
   connect( this, SIGNAL( triggered() ), this, SLOT( execute() ) );
}

///////////////////////////////////////////////////////////////////////////////

void MEAddEntityAction::execute()
{
   Entity* newEntity = m_addedClass.instantiate< Entity >();
   m_scene.add( newEntity );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

MERemoveEntityAction::MERemoveEntityAction( QWidget& parent, Model& scene, Entity* entityToRemove, const std::string& iconsDir )
: QAction( &parent )
, m_scene( scene )
, m_entityToRemove( entityToRemove )
{
   setText( "Delete" );
   setIcon( QIcon( ( iconsDir + "quit.png" ).c_str() ) );
   connect( this, SIGNAL( triggered() ), this, SLOT( execute() ) );
}

///////////////////////////////////////////////////////////////////////////////


void MERemoveEntityAction::execute()
{
   m_scene.remove( *m_entityToRemove );
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

MEClearAction::MEClearAction( QWidget& parent, Model& scene, const std::string& iconsDir )
: QAction( &parent )
, m_scene( scene )
{
   setText( "Clear" );
   setIcon( QIcon( ( iconsDir + "clear.png" ).c_str() ) );
   connect( this, SIGNAL( triggered() ), this, SLOT( execute() ) );
}

///////////////////////////////////////////////////////////////////////////////

void MEClearAction::execute()
{
   m_scene.clear();
}

///////////////////////////////////////////////////////////////////////////////
