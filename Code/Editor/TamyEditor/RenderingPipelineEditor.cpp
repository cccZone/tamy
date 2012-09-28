#include "RenderingPipelineEditor.h"
#include "core.h"
#include "core-Renderer.h"
#include "RenderingPipelineLayout.h"
#include "RenderTargetDescriptorDialog.h"
#include "DepthBufferDescriptorDialog.h"
#include "QPropertiesView.h"
#include "tamyeditor.h"
#include "RenderTargetMimeData.h"
#include <QMessageBox.h>
#include <QListWidget>


///////////////////////////////////////////////////////////////////////////////

RenderingPipelineEditor::RenderingPipelineEditor( RenderingPipelineLayout& renderingPipelineLayout )
   : m_renderingPipelineLayout( renderingPipelineLayout )
   , m_blockPropertiesLayout( NULL )
   , m_blockPropertiesRootView( NULL )
   , m_renderTargetsList( NULL )
{
   connect( &m_renderingPipelineLayout, SIGNAL( onBlockAdded() ), this, SLOT( onBlockAdded() ) );
   connect( &m_renderingPipelineLayout, SIGNAL( onBlockRemoved() ), this, SLOT( onBlockRemoved() ) );
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineEditor::~RenderingPipelineEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onInitialize()
{
   ResourcesManager& resourceMgr = ResourcesManager::getInstance();
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

   // configure the rendering targets list
   {
      m_ui.addTargetButton->setIcon( QIcon( iconsDir + tr( "/plus.png" ) ) );
      m_ui.removeTargetButton->setIcon( QIcon( iconsDir + tr( "/minus.png" ) ) );

      m_renderTargetsList = new QListWidget( this );
      m_ui.renderTargetsPageLayout->addWidget( m_renderTargetsList );

      connect( m_ui.addTargetButton, SIGNAL( clicked() ), this, SLOT( addRenderTarget() ) );
      connect( m_ui.removeTargetButton, SIGNAL( clicked() ), this, SLOT( removeRenderTarget() ) );
      connect( m_renderTargetsList, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( editRenderTarget( QListWidgetItem* ) ) );

      updateRenderTargetsList();
   }

   // configure the depth buffers list
   {
      m_ui.addDepthBufferButton->setIcon( QIcon( iconsDir + tr( "/plus.png" ) ) );
      m_ui.removeDepthBufferButton->setIcon( QIcon( iconsDir + tr( "/minus.png" ) ) );

      m_depthBuffersList = new QListWidget( this );
      m_ui.depthBuffersPageLayout->addWidget( m_depthBuffersList );

      connect( m_ui.addDepthBufferButton, SIGNAL( clicked() ), this, SLOT( addDepthBuffer() ) );
      connect( m_ui.removeDepthBufferButton, SIGNAL( clicked() ), this, SLOT( removeDepthBuffer() ) );
      connect( m_depthBuffersList, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( editDepthBuffer( QListWidgetItem* ) ) );

      updateDepthBuffersList();
   }

   // set the toolbar
   QToolBar* toolbar = new QToolBar( m_ui.toolbarFrame );
   m_ui.toolbarFrame->layout()->addWidget( toolbar );
   {
      QAction* actionSave = new QAction( QIcon( iconsDir + tr( "/saveFile.png" ) ), tr( "Save" ), toolbar );
      actionSave->setShortcut( QKeySequence( tr( "Ctrl+S" ) ) );
      toolbar->addAction( actionSave );
      connect( actionSave, SIGNAL( triggered() ), this, SLOT( save() ) );
   }

   // show the resource
   show();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onDeinitialize( bool saveProgress )
{
   if ( saveProgress )
   {
      save();
   }

   m_ui.graphWidget->setScene( NULL );

   if ( m_blockPropertiesLayout )
   {
      m_blockPropertiesLayout->removeWidget( m_blockPropertiesRootView );
      m_blockPropertiesLayout = NULL;
   }

   delete m_blockPropertiesRootView;
   m_blockPropertiesRootView = NULL;

   m_renderTargetsList = NULL;
   m_depthBuffersList = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::save()
{
   m_renderingPipelineLayout.saveResource();
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

   // handle selection of new item
   QList< QGraphicsItem* > selectedItems = m_renderingPipelineLayout.selectedItems();
   if ( selectedItems.size() != 1 )
   {
      return;
   }

   handleBlockSelection( dynamic_cast< GraphBlock* >( selectedItems.back() ) );
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

}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onShowContextMenu( const QPoint& pos )
{
   QMenu* contextMenu = new QMenu( m_ui.graphWidget );
   m_renderingPipelineLayout.createContextMenu( contextMenu, m_ui.graphWidget->mapToScene( pos ) );
   contextMenu->popup( m_ui.graphWidget->mapToGlobal( pos ) );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::updateRenderTargetsList()
{
   m_renderTargetsList->clear();

   const std::vector< RenderTargetDescriptor* >& descriptors = m_renderingPipelineLayout.getPipeline().getRenderTargets();
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

   RenderingPipelineTransaction transaction( m_renderingPipelineLayout.getPipeline() );
   if ( transaction.addRenderTarget( desc ) )
   {
      m_renderTargetsList->addItem( desc->getTargetID().c_str() );
   }
   transaction.commit();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::removeRenderTarget()
{
   RenderingPipeline& pipeline = m_renderingPipelineLayout.getPipeline();

   QList< QListWidgetItem* > itemsToRemove = m_renderTargetsList->selectedItems();
   RenderingPipelineTransaction transaction( pipeline );
   foreach( QListWidgetItem* item, itemsToRemove )
   {
      QString selectedTargetId = item->text();
      transaction.removeRenderTarget( selectedTargetId.toStdString() );
      
      delete item;
   }
   transaction.commit();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::editRenderTarget( QListWidgetItem* rtItem )
{
   RenderingPipeline& pipeline = m_renderingPipelineLayout.getPipeline();
   RenderTargetDescriptor& desc = pipeline.lockRenderTarget( rtItem->text().toStdString() );

   RenderTargetDescriptorDialog dialog( this, desc, false );
   dialog.exec();
   pipeline.unlockRenderTarget( rtItem->text().toStdString() );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::updateDepthBuffersList()
{
   m_depthBuffersList->clear();

   const std::vector< DepthBufferDescriptor* >& descriptors = m_renderingPipelineLayout.getPipeline().getDepthBuffers();
   unsigned int count = descriptors.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_depthBuffersList->addItem( descriptors[i]->getID().c_str() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::addDepthBuffer()
{
   DepthBufferDescriptor* desc = new DepthBufferDescriptor();
   DepthBufferDescriptorDialog dialog( this, *desc );
   if ( dialog.exec() != QDialog::Accepted )
   {
      delete desc;
      return;
   }

   if ( desc->getID().empty() )
   {
      delete desc;
      return;
   }

   RenderingPipelineTransaction transaction( m_renderingPipelineLayout.getPipeline() );
   if ( transaction.addDepthBuffer( desc ) )
   {
      m_depthBuffersList->addItem( desc->getID().c_str() );
   }
   transaction.commit();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::removeDepthBuffer()
{
   RenderingPipeline& pipeline = m_renderingPipelineLayout.getPipeline();

   QList< QListWidgetItem* > itemsToRemove = m_depthBuffersList->selectedItems();
   RenderingPipelineTransaction transaction( pipeline );
   foreach( QListWidgetItem* item, itemsToRemove )
   {
      QString selectedBufferId = item->text();
      transaction.removeDepthBuffer( selectedBufferId.toStdString() );

      delete item;
   }
   transaction.commit();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::editDepthBuffer( QListWidgetItem* dbItem )
{
   RenderingPipeline& pipeline = m_renderingPipelineLayout.getPipeline();
   DepthBufferDescriptor& desc = pipeline.lockDepthBuffer( dbItem->text().toStdString() );

   DepthBufferDescriptorDialog dialog( this, desc, false );
   dialog.exec();
   pipeline.unlockDepthBuffer( dbItem->text().toStdString() );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onBlockAdded()
{
   // Block addition could have introduced new render targets and depth buffers.
   // Refresh the respective lists
   updateRenderTargetsList();
   updateDepthBuffersList();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onBlockRemoved()
{
   // Block addition could have removed some of the existing render targets and depth buffers.
   // Refresh the respective lists
   updateRenderTargetsList();
   updateDepthBuffersList();
}

///////////////////////////////////////////////////////////////////////////////
