#include "RenderingPipelineEditor.h"
#include "core.h"
#include "core-Renderer.h"
#include "RenderingPipelineLayout.h"
#include "RenderTargetDescriptorDialog.h"
#include "QPropertiesView.h"
#include "tamyeditor.h"
#include "RenderTargetMimeData.h"
#include <QCloseEvent>
#include <QMessageBox.h>
#include <QListWidget>


///////////////////////////////////////////////////////////////////////////////

RenderingPipelineEditor::RenderingPipelineEditor( RenderingPipelineLayout& renderingPipelineLayout )
: QMainWindow( NULL, 0 )
, m_renderingPipelineLayout( renderingPipelineLayout )
, m_blockPropertiesLayout( NULL )
, m_blockPropertiesRootView( NULL )
, m_nodePropertiesLayout( NULL )
, m_nodePropertiesRootView( NULL )
, m_renderTargetsList( NULL )
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

   m_renderTargetsList = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::initialize( TamyEditor& mgr )
{
   ResourcesManager& resourceMgr = mgr.requestService< ResourcesManager >();
   QString iconsDir = resourceMgr.getFilesystem().getShortcut( "editorIcons" ).c_str();

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

   // configure the rendering targets list
   {
      m_ui.addTargetButton->setIcon( QIcon( iconsDir + tr( "/plus.png" ) ) );
      m_ui.removeTargetButton->setIcon( QIcon( iconsDir + tr( "/minus.png" ) ) );
      
      connect( m_ui.addTargetButton, SIGNAL( clicked() ), this, SLOT( addRenderTarget() ) );
      connect( m_ui.removeTargetButton, SIGNAL( clicked() ), this, SLOT( removeRenderTarget() ) );

      m_renderTargetsList = new RenderTargetsListWidget( this );
      m_ui.renderTargetsPageLayout->addWidget( m_renderTargetsList );

      updateRenderTargetsList();
   }
   

   // set the menu
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
   // cleanup previous selection
   if ( m_blockPropertiesRootView != NULL )
   {
      m_blockPropertiesLayout->removeWidget( m_blockPropertiesRootView );

      delete m_blockPropertiesRootView;
      m_blockPropertiesRootView = NULL;
   }

   if ( m_nodePropertiesRootView != NULL )
   {
      m_nodePropertiesLayout->removeWidget( m_nodePropertiesRootView );

      delete m_nodePropertiesRootView;
      m_nodePropertiesRootView = NULL;
   }

   // handle selection of new item
   QList< QGraphicsItem* > selectedItems = m_renderingPipelineLayout.selectedItems();
   if ( selectedItems.size() != 1 )
   {
      return;
   }

   handleBlockSelection( dynamic_cast< GraphBlock* >( selectedItems.back() ) );
   handleSocketSelection( dynamic_cast< GraphBlockSocket* >( selectedItems.back() ) );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::handleBlockSelection( GraphBlock* selectedBlock )
{
   if ( !selectedBlock )
   {
      return;
   }

   // block properties
   m_blockPropertiesRootView = new QPropertiesView();
   m_blockPropertiesLayout->addWidget( m_blockPropertiesRootView );
   selectedBlock->viewProperties( *m_blockPropertiesRootView );

   // node properties
   Object& node = selectedBlock->getNode();
   m_nodePropertiesRootView = new QPropertiesView();
   m_nodePropertiesLayout->addWidget( m_nodePropertiesRootView );
   node.viewProperties( *m_nodePropertiesRootView );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::handleSocketSelection( GraphBlockSocket* selectedSocket )
{
   if ( !selectedSocket )
   {
      return;
   }

   // node properties
   Object& node = selectedSocket->getSocket();
   m_nodePropertiesRootView = new QPropertiesView();
   m_nodePropertiesLayout->addWidget( m_nodePropertiesRootView );
   node.viewProperties( *m_nodePropertiesRootView );
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

void RenderingPipelineEditor::updateRenderTargetsList()
{
   m_renderTargetsList->clear();

   const std::vector< RenderTargetDescriptor* >& descriptors = m_renderingPipelineLayout.getModel().getRenderTargets();
   unsigned int count = descriptors.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
     m_renderTargetsList->addItem( descriptors[i]->getTargetID().c_str() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::addRenderTarget()
{
   RenderTargetDescriptor* desc = new RenderTargetDescriptor();
   RenderTargetDescriptorDialog dialog( this, *desc );
   if ( dialog.exec() != QDialog::Accepted )
   {
      delete desc;
      return;
   }

   if ( desc->getTargetID().empty() )
   {
      delete desc;
      return;
   }

   RenderingPipeline& pipeline = m_renderingPipelineLayout.getModel();
   if ( pipeline.addRenderTarget( desc ) )
   {
      m_renderTargetsList->addItem( desc->getTargetID().c_str() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::removeRenderTarget()
{
   RenderingPipeline& pipeline = m_renderingPipelineLayout.getModel();

   QList< QListWidgetItem* > itemsToRemove = m_renderTargetsList->selectedItems();
   foreach( QListWidgetItem* item, itemsToRemove )
   {
      QString selectedTargetId = item->text();
      pipeline.removeRenderTarget( selectedTargetId.toStdString() );
      
      delete item;
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RenderTargetsListWidget::RenderTargetsListWidget( QWidget* parent )
   : QListWidget( parent )
{
   setDragEnabled( true );
   setDragDropMode( QAbstractItemView::DragOnly );
}

///////////////////////////////////////////////////////////////////////////////

QMimeData* RenderTargetsListWidget::mimeData( const QList< QListWidgetItem* > items ) const
{
   QMimeData* data = __super::mimeData( items );

   std::string selectedTargetID;
   foreach( QListWidgetItem* item, items )
   {
      selectedTargetID = item->text().toStdString();
      break;
   }

   RenderTargetMimeData dataEncoder( selectedTargetID );
   dataEncoder.save( *data );

   return data;
}

///////////////////////////////////////////////////////////////////////////////
