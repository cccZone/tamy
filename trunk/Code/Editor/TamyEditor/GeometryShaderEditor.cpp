#include "GeometryShaderEditor.h"
#include "core.h"
#include "core-Renderer.h"
#include "GeometryShaderLayout.h"
#include "QPropertiesView.h"
#include "tamyeditor.h"
#include <QMessageBox.h>


///////////////////////////////////////////////////////////////////////////////

GeometryShaderEditor::GeometryShaderEditor( GeometryShaderLayout& geometryShaderLayout )
   : m_geometryShaderLayout( geometryShaderLayout )
   , m_blockPropertiesLayout( NULL )
   , m_blockPropertiesRootView( NULL )
   , m_nodePropertiesLayout( NULL )
   , m_nodePropertiesRootView( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

GeometryShaderEditor::~GeometryShaderEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderEditor::onInitialize()
{
   ResourcesManager& resourceMgr = ResourcesManager::getInstance();
   QString iconsDir = resourceMgr.getFilesystem().getShortcut( "editorIcons" ).c_str();

   // setup the UI
   m_ui.setupUi( this );

   // attach the graph viewer
   m_ui.graphWidget->setScene( &m_geometryShaderLayout );
   connect( m_ui.graphWidget, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( onShowContextMenu( const QPoint& ) ) );

   // set handlers for the signals emitted by the scene
   connect( &m_geometryShaderLayout, SIGNAL( selectionChanged() ), this, SLOT( onSceneSelectionChanged() ) );

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

void GeometryShaderEditor::onDeinitialize( bool saveProgress )
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

   if ( m_nodePropertiesLayout )
   {
      m_nodePropertiesLayout->removeWidget( m_nodePropertiesRootView );
      m_nodePropertiesLayout = NULL;
   }

   delete m_nodePropertiesRootView;
   m_nodePropertiesRootView = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderEditor::save()
{
   m_geometryShaderLayout.saveResource();
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderEditor::onSceneSelectionChanged()
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
   QList< QGraphicsItem* > selectedItems = m_geometryShaderLayout.selectedItems();
   if ( selectedItems.size() != 1 )
   {
      return;
   }

   handleBlockSelection( dynamic_cast< GraphBlock* >( selectedItems.back() ) );
   handleSocketSelection( dynamic_cast< GraphBlockSocket* >( selectedItems.back() ) );
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderEditor::handleBlockSelection( GraphBlock* selectedBlock )
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
   ReflectionObject& node = selectedBlock->getNode();
   m_nodePropertiesRootView = new QPropertiesView();
   m_nodePropertiesLayout->addWidget( m_nodePropertiesRootView );
   node.viewProperties( *m_nodePropertiesRootView );
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderEditor::handleSocketSelection( GraphBlockSocket* selectedSocket )
{
   if ( !selectedSocket )
   {
      return;
   }

   // node properties
   ReflectionObject& node = selectedSocket->getSocket();
   m_nodePropertiesRootView = new QPropertiesView();
   m_nodePropertiesLayout->addWidget( m_nodePropertiesRootView );
   node.viewProperties( *m_nodePropertiesRootView );
}

///////////////////////////////////////////////////////////////////////////////

void GeometryShaderEditor::onShowContextMenu( const QPoint& pos )
{
   QMenu* contextMenu = new QMenu( m_ui.graphWidget );
   m_geometryShaderLayout.createContextMenu( contextMenu, m_ui.graphWidget->mapToScene( pos ) );
   contextMenu->popup( m_ui.graphWidget->mapToGlobal( pos ) );
}

///////////////////////////////////////////////////////////////////////////////
