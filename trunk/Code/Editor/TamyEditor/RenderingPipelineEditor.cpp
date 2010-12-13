#include "RenderingPipelineEditor.h"
#include "core.h"
#include "core-Renderer.h"
#include "RenderingPipelineLayout.h"
#include "QPropertiesView.h"
#include "tamyeditor.h"
#include <QCloseEvent>
#include <QMessageBox.h>


///////////////////////////////////////////////////////////////////////////////

RenderingPipelineEditor::RenderingPipelineEditor( RenderingPipelineLayout& renderingPipelineLayout )
: QMainWindow( NULL, 0 )
, m_renderingPipelineLayout( renderingPipelineLayout )
, m_blockPropertiesLayout( NULL )
, m_blockPropertiesRootView( NULL )
, m_nodePropertiesLayout( NULL )
, m_nodePropertiesRootView( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineEditor::~RenderingPipelineEditor()
{
   if ( m_blockPropertiesLayout )
   {
      m_blockPropertiesLayout->removeWidget( m_blockPropertiesRootView );
      m_blockPropertiesLayout = NULL;
   }

   delete m_blockPropertiesRootView;
   m_blockPropertiesRootView = NULL;

   if ( m_nodePropertiesLayout )
   {
      m_nodePropertiesLayout->removeWidget( m_nodePropertiesRootView );
      m_nodePropertiesLayout = NULL;
   }

   delete m_nodePropertiesRootView;
   m_nodePropertiesRootView = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::initialize( TamyEditor& mgr )
{
   // setup the UI
   m_ui.setupUi( this );
   
   // attach the graph viewer
   m_ui.graphWidget->setScene( &m_renderingPipelineLayout );
   connect( m_ui.graphWidget, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( onShowContextMenu( const QPoint& ) ) );

   // set handlers for the signals emitted by the scene
   connect( &m_renderingPipelineLayout, SIGNAL( selectionChanged() ), this, SLOT( onSceneSelectionChanged() ) );

   // attach the block properties viewer
   m_blockPropertiesLayout = new QVBoxLayout( m_ui.scrollableBlockPropertiesPanel );
   m_ui.scrollableBlockPropertiesPanel->setLayout( m_blockPropertiesLayout );
   m_blockPropertiesLayout->setSpacing(0);
   m_blockPropertiesLayout->setMargin(0);

   // attach the node properties viewer
   m_nodePropertiesLayout = new QVBoxLayout( m_ui.scrollableNodePropertiesPanel );
   m_ui.scrollableNodePropertiesPanel->setLayout( m_nodePropertiesLayout );
   m_nodePropertiesLayout->setSpacing(0);
   m_nodePropertiesLayout->setMargin(0);


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

void RenderingPipelineEditor::onSceneSelectionChanged()
{
   GraphBlock* selectedBlock = NULL;

   QList< QGraphicsItem* > selectedItems = m_renderingPipelineLayout.selectedItems();
   if ( selectedItems.size() == 1 )
   {
      selectedBlock = dynamic_cast< GraphBlock* >( selectedItems.back() );
   }

   // block properties
   {
      if ( m_blockPropertiesRootView != NULL )
      {
         m_blockPropertiesLayout->removeWidget( m_blockPropertiesRootView );

         delete m_blockPropertiesRootView;
         m_blockPropertiesRootView = NULL;
      }

      if ( selectedBlock != NULL )
      {
         m_blockPropertiesRootView = new QPropertiesView();
         m_blockPropertiesLayout->addWidget( m_blockPropertiesRootView );
         selectedBlock->viewProperties( *m_blockPropertiesRootView );
      }
   }

   // node properties
   {
      if ( m_nodePropertiesRootView != NULL )
      {
         m_nodePropertiesLayout->removeWidget( m_nodePropertiesRootView );

         delete m_nodePropertiesRootView;
         m_nodePropertiesRootView = NULL;
      }

      if ( selectedBlock != NULL )
      {
         Object& node = selectedBlock->getNode();
         m_nodePropertiesRootView = new QPropertiesView();
         m_nodePropertiesLayout->addWidget( m_nodePropertiesRootView );
         node.viewProperties( *m_nodePropertiesRootView );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onShowContextMenu( const QPoint& pos )
{
   QMenu* contextMenu = new QMenu( m_ui.graphWidget );
   m_renderingPipelineLayout.createContextMenu( contextMenu, m_ui.graphWidget->mapToScene( pos ) );
   contextMenu->popup( m_ui.graphWidget->mapToGlobal( pos ) );
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
