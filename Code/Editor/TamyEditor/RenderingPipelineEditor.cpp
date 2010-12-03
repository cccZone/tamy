#include "RenderingPipelineEditor.h"
#include "core.h"
#include "core-Renderer.h"
#include "GraphWidget.h"
#include "GraphBlock.h"
#include "RenderingPipelineLayout.h"
#include "tamyeditor.h"
#include <QCloseEvent>
#include <QMessageBox.h>


///////////////////////////////////////////////////////////////////////////////

RenderingPipelineEditor::RenderingPipelineEditor( RenderingPipelineLayout& renderingPipelineLayout )
: QMainWindow( NULL, 0 )
, m_renderingPipelineLayout( renderingPipelineLayout )
, m_graphWidget( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::initialize( TamyEditor& mgr )
{
   // setup the UI
   m_ui.setupUi( this );
   
   m_graphWidget = new GraphWidget( m_ui.mainEditorPanel, m_renderingPipelineLayout );
   m_ui.mainEditorPanel->setWidget( m_graphWidget );
   connect( m_graphWidget, SIGNAL( getNodesClasses( std::vector< Class >& ) ), this, SLOT( onGetNodesClasses( std::vector< Class >& ) ) );
   connect( m_graphWidget, SIGNAL( getEdgesClasses( std::vector< Class >& ) ), this, SLOT( onGetEdgesClasses( std::vector< Class >& ) ) );
   connect( m_graphWidget, SIGNAL( popupMenuShown( QMenu& ) ), this, SLOT( onPopupMenuShown( QMenu& ) ) );

   // set the menu
   ResourcesManager& resourceMgr = mgr.requestService< ResourcesManager >();
   QString iconsDir = resourceMgr.getFilesystem().getShortcut( "editorIcons" ).c_str();

   QMenu* fileMenu = m_ui.menubar->addMenu( "File" );
   {
      QAction* actionSave = new QAction( QIcon( iconsDir + tr( "/saveFile.png" ) ), tr( "Save" ), fileMenu );
      actionSave->setShortcut( QKeySequence( tr( "Ctrl+S" ) ) );
      fileMenu->addAction( actionSave );
      m_ui.toolBar->addAction( actionSave );
      connect( actionSave, SIGNAL( triggered() ), this, SLOT( save() ) );

      QAction* separator = new QAction( fileMenu );
      separator->setSeparator( true );
      fileMenu->addAction( separator );
      m_ui.toolBar->addAction( separator );

      QAction* actionExit = new QAction( QIcon( iconsDir + tr( "/quit.png" ) ), tr( "Exit" ), this );
      actionExit->setShortcut( QKeySequence( tr( "Ctrl+Q" ) ) );
      fileMenu->addAction( actionExit );
      connect( actionExit, SIGNAL( triggered() ), this, SLOT( close() ) );
   }

   // configure the editor to display nodes in certain way
   m_graphWidget->associate< TestNode >( GraphBlock::GBS_RECTANGLE, QColor( 100, 200, 100 )  );

   // show the resource
   show();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::save()
{
   ExternalDependenciesSet externalDependencies;
   m_renderingPipelineLayout.saveResource( externalDependencies );

   for ( unsigned int i = 0; i < externalDependencies.size(); ++i )
   {
      externalDependencies[ i ]->saveResource( externalDependencies );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onGetNodesClasses( std::vector< Class >& classes )
{
   ClassesRegistry& classesReg = getClassesRegistry();
   classesReg.getClassesMatchingType< TestNode >( classes );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onGetEdgesClasses( std::vector< Class >& classes )
{
   ClassesRegistry& classesReg = getClassesRegistry();
   classesReg.getClassesMatchingType< TestEdge >( classes );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onPopupMenuShown( QMenu& menu )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::closeEvent( QCloseEvent *event )
{
   save();

   // accept the event
   event->accept();

   delete this;
}

///////////////////////////////////////////////////////////////////////////////

// TODO:
// 1.) dokonczyc widget
// 2.) edycja parametrow node'a
