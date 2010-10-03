#include "SceneTreeViewer.h"
#include "tamyeditor.h"
#include <QWidget.h>
#include <QDockWidget.h>
#include <QBoxLayout.h>
#include <QTreeWidget.h>
#include <QObject.h>
#include <QMouseEvent>
#include "core-MVC.h"
#include "core\Assert.h"
#include "core-Renderer.h"
#include "TypeDescFactory.h"
#include "TreeWidget.h"

// editors
#include "ModelEditor.h"
#include "EntityEditor.h"


///////////////////////////////////////////////////////////////////////////////

SceneTreeViewer::SceneTreeViewer()
: m_mgr( NULL )
, m_sceneTree( NULL )
, m_observedScene( NULL )
, m_rootItem( NULL )
, m_selectionMgr( NULL )
, m_camera( NULL )
, m_itemsFactory( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

SceneTreeViewer::~SceneTreeViewer()
{
   m_mgr = NULL;
   m_sceneTree = NULL;
   m_rootItem = NULL;
   m_observedScene = NULL;
   m_selectionMgr = NULL;
   m_camera = NULL;

   delete m_itemsFactory;
   m_itemsFactory = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::initialize( TamyEditor& mgr )
{
   ASSERT_MSG( m_mgr == NULL, "SceneTreeViewer component is already initialized" );
   m_mgr = &mgr;

   ResourcesManager& resMgr = mgr.requestService< ResourcesManager >();
   const Filesystem& fs = resMgr.getFilesystem();
   m_iconsDir = fs.getShortcut( "editorIcons" ).c_str();

   m_itemsFactory = new TypeDescFactory< Entity >( m_iconsDir, fs, "unknownTypeIcon.png" );

   // initialize user interface
   initUI( mgr );
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::onServiceRegistered( TamyEditor& mgr )
{
   // scene
   if ( mgr.needsUpdate< Model >( *m_observedScene ) )
   {
      if ( m_observedScene )
      {
         m_observedScene->detach( *this );
      }

      if ( mgr.hasService< Model >() )
      {
         m_observedScene = &mgr.requestService< Model >();
         m_observedScene->attach( *this );
      }
      else
      {
         m_observedScene = NULL;
      }
   }

   // selection manager
   if ( mgr.needsUpdate< SelectionManager >( *m_selectionMgr ) )
   {
      if ( m_selectionMgr )
      {
         m_selectionMgr->detach( *this );
      }

      if ( mgr.hasService< SelectionManager >() )
      {
         m_selectionMgr = &mgr.requestService< SelectionManager >();
         m_selectionMgr->attach( *this );
      }
      else
      {
         m_selectionMgr = NULL;
      }
   }

   // scene navigator
   if ( mgr.needsUpdate< Camera >( *m_camera ) )
   {
      if ( mgr.hasService< Camera >() )
      {
         m_camera = &mgr.requestService< Camera >();
      }
      else
      {
         m_camera = NULL;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::initUI( TamyEditor& mgr )
{
   // setup dockable properties view widget
   QDockWidget* dockWidget = new QDockWidget( "Scene Tree", &mgr );
   dockWidget->setObjectName("SceneTreeViewer/dockWidget");
   mgr.addDockWidget( static_cast<Qt::DockWidgetArea>(2), dockWidget );

   QWidget* dockWidgetContents = new QWidget( &mgr );
   dockWidgetContents->setObjectName("SceneTreeViewer/dockWidgetContents");
   dockWidget->setWidget( dockWidgetContents );

   QVBoxLayout* layout = new QVBoxLayout( dockWidgetContents );
   dockWidgetContents->setLayout( layout );
   layout->setSpacing(0);
   layout->setMargin(0);

   // setup the scene tree container widget
   m_sceneTree = new TreeWidget( &mgr, "SceneTreeViewer/m_sceneTree", m_iconsDir );
   layout->addWidget( m_sceneTree );

   QStringList columnLabels; 
   columnLabels.push_back( "Name" );
   columnLabels.push_back( "Type" );
   m_sceneTree->setColumnCount( columnLabels.size() );
   m_sceneTree->setHeaderLabels( columnLabels );
   connect( m_sceneTree, SIGNAL( itemClicked( QTreeWidgetItem*, int ) ), this, SLOT( selectItem( QTreeWidgetItem*, int ) ) );
   connect( m_sceneTree, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( focusOnItem( QTreeWidgetItem*, int ) ) );
   connect( m_sceneTree, SIGNAL( getItemsFactory( QTreeWidgetItem*, TreeWidgetDescFactory*& ) ), this, SLOT( getItemsFactory( QTreeWidgetItem*, TreeWidgetDescFactory*& ) ) );
   connect( m_sceneTree, SIGNAL( addNode( QTreeWidgetItem*, unsigned int ) ), this, SLOT( addNode( QTreeWidgetItem*, unsigned int ) ) );
   connect( m_sceneTree, SIGNAL( removeNode( QTreeWidgetItem*, QTreeWidgetItem* ) ), this, SLOT( removeNode( QTreeWidgetItem*, QTreeWidgetItem* ) ) );
   connect( m_sceneTree, SIGNAL( clearNode( QTreeWidgetItem* ) ), this, SLOT( clearNode( QTreeWidgetItem* ) ) );

   m_rootItem = new EntityTreeItem( m_sceneTree, mgr );
   m_sceneTree->addTopLevelItem( m_rootItem );

   // setup menu entries
   QAction* actionProperties = dockWidget->toggleViewAction();
   actionProperties->setText( QApplication::translate( "TamyEditorClass", "Scene Tree", 0, QApplication::UnicodeUTF8 ) );
   mgr.getViewMenu().addAction( actionProperties );
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::onEntityAdded( Entity& entity )
{
   if ( !m_rootItem || !m_mgr )
   {
      return;
   }

   std::list< Entity* > entitiesStack;
   buildEntitiesStack( entity, entitiesStack );

   // find the proper place in the tree for it
   EntityTreeItem* currItem = m_rootItem;

   for ( std::list< Entity* >::iterator it = entitiesStack.begin();
      it != entitiesStack.end(); ++it )
   {
      Entity* currEntity = *it;

      // find a matching entity at this level
      EntityTreeItem* matchingItem = currItem->find( currEntity );

      if ( matchingItem )
      {
         currItem = matchingItem;
      }
      else
      {
         // insert the item
         currItem = new EntityTreeItem( currEntity, currItem, *m_itemsFactory );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::onEntityRemoved( Entity& entity )
{
   if ( !m_rootItem )
   {
      return;
   }

   EntityTreeItem* entry = find( entity );
   if ( entry )
   {
      entry->parent()->removeChild( entry );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::onEntityChanged( Entity& entity )
{
   if ( !m_rootItem )
   {
      return;
   }

   EntityTreeItem* entry = find( entity );
   entry->update();
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::resetContents()
{
   if ( !m_rootItem )
   {
      return;
   }

   int count = m_rootItem->childCount();
   for ( int i = 0; i < count; ++i )
   {
      m_rootItem->removeChild( m_rootItem->child( 0 ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::buildEntitiesStack( Entity& entity, std::list< Entity* >& stack ) const
{
   stack.push_front( &entity );
   while( stack.front()->isAttached() )
   {
      stack.push_front( &stack.front()->getParent() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::selectItem( QTreeWidgetItem* item, int column )
{
   if ( !m_selectionMgr )
   {
      return;
   }

   EntityTreeItem* entityItem = dynamic_cast< EntityTreeItem* >( item );
   if ( !entityItem )
   {
      return;
   }

   if ( entityItem->getEntity() )
   {
      m_selectionMgr->selectObject( *entityItem->getEntity() );
   }
   else
   {
      m_selectionMgr->resetSelection();
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::focusOnItem( QTreeWidgetItem* item, int column )
{
   selectItem( item, column );

   if ( !m_camera )
   {
      return;
   }

   EntityTreeItem* entityItem = dynamic_cast< EntityTreeItem* >( item );
   if ( !entityItem )
   {
      return;
   }

   SpatialEntity* spatial = dynamic_cast< SpatialEntity* >( entityItem->getEntity() );
   if ( spatial )
   {
      m_camera->lookAt( *spatial, 5.f );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::getItemsFactory( QTreeWidgetItem* parent, TreeWidgetDescFactory*& outFactoryPtr )
{
   outFactoryPtr = m_itemsFactory;
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::addNode( QTreeWidgetItem* parent, unsigned int typeIdx )
{
   if ( !m_observedScene )
   {
      return;
   }

   SceneTreeEditor* editor = createEditor( dynamic_cast< EntityTreeItem* >( parent ) );
   editor->addEntity( m_itemsFactory->getClass( typeIdx ) );
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::removeNode( QTreeWidgetItem* parent, QTreeWidgetItem* child )
{
   if ( !m_observedScene )
   {
      return;
   }

   SceneTreeEditor* parentEditor = createEditor( dynamic_cast< EntityTreeItem* >( parent ) );
   EntityTreeItem* item = dynamic_cast< EntityTreeItem* >( child );
   parentEditor->removeEntity( item->getEntity() );
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::clearNode( QTreeWidgetItem* node )
{
   if ( !m_observedScene )
   {
      return;
   }

   SceneTreeEditor* editor = createEditor( dynamic_cast< EntityTreeItem* >( node ) );
   editor->clearEntity();
}

///////////////////////////////////////////////////////////////////////////////

SceneTreeEditor* SceneTreeViewer::createEditor( EntityTreeItem* item )
{
   SceneTreeEditor* editor = NULL;

   if ( item )
   {
      Entity* entity = item->getEntity();
      if ( entity )
      {
         editor = new EntityEditor( *entity );
      }
      else
      {
         editor = new ModelEditor( *m_observedScene );
      }
   }
   else
   {
      editor = new ModelEditor( *m_observedScene );
   }

   return editor;
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::onObjectSelected( Entity& entity )
{
   EntityTreeItem* entityItem = find( entity );
   if ( entityItem )
   {
      m_sceneTree->setItemSelected( entityItem, true );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::onObjectDeselected( Entity& entity )
{
   EntityTreeItem* entityItem = find( entity );
   if ( entityItem )
   {
      m_sceneTree->setItemSelected( entityItem, false );
   }
}

///////////////////////////////////////////////////////////////////////////////

SceneTreeViewer::EntityTreeItem* SceneTreeViewer::find( Entity& entity )
{
   std::list< Entity* > entitiesStack;
   buildEntitiesStack( entity, entitiesStack );

   // find the entity and remove it from the list
   EntityTreeItem* currItem = m_rootItem;

   for ( std::list< Entity* >::iterator it = entitiesStack.begin();
      it != entitiesStack.end(); ++it )
   {
      Entity* currEntity = *it;

      // find a matching entity at this level
      EntityTreeItem* matchingItem = currItem->find( currEntity );

      if ( !matchingItem )
      {
         // entity not found
         break;
      }

      // if we got here, it means we found an entry for the entity.
      // Now we need to establish if that's the last entry in the chain,
      // in which case we can remove it from the tree, or should we dig deeper.
      if ( matchingItem->getEntity() == &entity )
      {
         return matchingItem;
      }
      else
      {
         currItem = matchingItem;
      }
   }

   // we didn't manage to find the entity
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SceneTreeViewer::EntityTreeItem::EntityTreeItem( QTreeWidget* parent, TamyEditor& mgr )
: QTreeWidgetItem( parent )
, m_entity( NULL )
{
   setText( 0, "World" );
   setText( 1, "Model" );

   ResourcesManager& resMgr = mgr.requestService< ResourcesManager >();
   QString iconsDir = resMgr.getFilesystem().getShortcut( "editorIcons" ).c_str();
   setIcon( 0, QIcon( iconsDir + "/world.png" ) );
}

///////////////////////////////////////////////////////////////////////////////

SceneTreeViewer::EntityTreeItem::EntityTreeItem( Entity* entity, 
                                                 QTreeWidgetItem* parent,
                                                 TypeDescFactory< Entity >& itemsFactory )
: QTreeWidgetItem( parent )
, m_entity( entity )
{
   ASSERT_MSG( entity != NULL, "Entity can't be NULL" );

   QString typeDesc;
   QIcon icon;
   Class virtType = entity->getVirtualClass();
   itemsFactory.getDesc( virtType, typeDesc, icon );

   setText( 0, getEntityName( entity ) );
   setText( 1, typeDesc );
   setIcon( 0, icon );
}

///////////////////////////////////////////////////////////////////////////////

QString SceneTreeViewer::EntityTreeItem::getEntityName( Entity* entity ) const
{
   SpatialEntity* spatial;
   Geometry* geometry;

   if ( ( geometry = dynamic_cast< Geometry* >( entity ) ) != NULL )
   {
      return geometry->getGeometryName().c_str();
   }
   else
   {
      if ( entity->getEntityName().empty() )
      {
         return "<<anonymous>>";
      }
      else
      {
         return entity->getEntityName().c_str();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::EntityTreeItem::update()
{
   setText( 0, getEntityName( m_entity ) );
}

///////////////////////////////////////////////////////////////////////////////

SceneTreeViewer::EntityTreeItem* SceneTreeViewer::EntityTreeItem::find( Entity* entity )
{
   int count = childCount();
   for ( int i = 0; i < count; ++i )
   {
      EntityTreeItem* thisItem = dynamic_cast< EntityTreeItem* >( child( i ) );
      if ( thisItem->getEntity() == entity )
      {
         return thisItem;
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
