#include "RenderingPipelineEditor.h"
#include "core.h"
#include "core-Renderer.h"
#include "GraphWidget.h"
#include "RenderingPipelineLayout.h"
#include "QPropertiesView.h"
#include "tamyeditor.h"
#include <QCloseEvent>
#include <QMessageBox.h>


///////////////////////////////////////////////////////////////////////////////

RenderingPipelineEditor::RenderingPipelineEditor( RenderingPipelineLayout& renderingPipelineLayout )
: QMainWindow( NULL, 0 )
, m_renderingPipelineLayout( renderingPipelineLayout )
, m_graphWidget( NULL )
, m_propertiesLayout( NULL )
, m_rootView( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineEditor::~RenderingPipelineEditor()
{
   if ( m_propertiesLayout )
   {
      m_propertiesLayout->removeWidget( m_rootView );
      m_propertiesLayout = NULL;
   }

   delete m_rootView;
   m_rootView = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::initialize( TamyEditor& mgr )
{
   // setup the UI
   m_ui.setupUi( this );
   
   // attach the graph viewer
   m_graphWidget = new GraphWidget( m_ui.mainEditorPanel, m_renderingPipelineLayout );
   m_ui.mainEditorPanel->setWidget( m_graphWidget );
   connect( m_graphWidget, SIGNAL( getNodesClasses( std::vector< Class >& ) ), this, SLOT( onGetNodesClasses( std::vector< Class >& ) ) );
   connect( m_graphWidget, SIGNAL( getEdgesClasses( std::vector< Class >& ) ), this, SLOT( onGetEdgesClasses( std::vector< Class >& ) ) );
   connect( m_graphWidget, SIGNAL( popupMenuShown( QMenu& ) ), this, SLOT( onPopupMenuShown( QMenu& ) ) );
   connect( m_graphWidget, SIGNAL( blockSelected( Object* ) ), this, SLOT( onBlockSelected( Object* ) ) );

   // attach the properties viewer
   m_propertiesLayout = new QVBoxLayout( m_ui.scrollablePropertiesPanel );
   m_ui.scrollablePropertiesPanel->setLayout( m_propertiesLayout );
   m_propertiesLayout->setSpacing(0);
   m_propertiesLayout->setMargin(0);


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

void RenderingPipelineEditor::onPopupMenuShown( QMenu& menu )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onBlockSelected( Object* object )
{
  
   if ( m_rootView != NULL )
   {
      m_propertiesLayout->removeWidget( m_rootView );

      delete m_rootView;
      m_rootView = NULL;
   }

   if ( object != NULL )
   {
      m_rootView = new QPropertiesView();
      m_propertiesLayout->addWidget( m_rootView );
      object->viewProperties( *m_rootView );
   }
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
