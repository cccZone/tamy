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
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::initialize( TamyEditor& mgr )
{
   ASSERT( m_mgr == NULL, "SceneTreeViewer component is already initialized" );
   m_mgr = &mgr;

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
   mgr.addDockWidget( static_cast<Qt::DockWidgetArea>(2), dockWidget );

   QWidget* dockWidgetContents = new QWidget( &mgr );
   dockWidget->setWidget( dockWidgetContents );

   QVBoxLayout* layout = new QVBoxLayout( dockWidgetContents );
   dockWidgetContents->setLayout( layout );
   layout->setSpacing(0);
   layout->setMargin(0);

   // setup the scene tree container widget
   m_sceneTree = new QTreeWidget( &mgr );
   layout->addWidget( m_sceneTree );

   QStringList columnLabels; 
   columnLabels.push_back( "Name" );
   columnLabels.push_back( "Type" );
   m_sceneTree->setColumnCount( columnLabels.size() );
   m_sceneTree->setHeaderLabels( columnLabels );
   m_sceneTree->setContextMenuPolicy( Qt::CustomContextMenu );
   connect( m_sceneTree, SIGNAL( itemClicked( QTreeWidgetItem*, int ) ), this, SLOT( selectItem( QTreeWidgetItem*, int ) ) );
   connect( m_sceneTree, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( focusOnItem( QTreeWidgetItem*, int ) ) );
   connect( m_sceneTree, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( showPopupMenu( const QPoint& ) ) );

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
         currItem = new EntityTreeItem( currEntity, currItem, *m_mgr );
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

void SceneTreeViewer::showPopupMenu( const QPoint& pos )
{
   if ( !m_observedScene )
   {
      return;
   }

   EntityTreeItem* item = dynamic_cast< EntityTreeItem* >( m_sceneTree->itemAt( pos ) );
   SceneTreeEditor* parentEditor = NULL;
   SceneTreeEditor* entityEditor = createEditor( item );
   if ( item )
   {
      parentEditor = createEditor( dynamic_cast< EntityTreeItem* >( item->parent() ) );
   }  

   QMenu* popupMenu = new QMenu( m_sceneTree );

   // populate with classes we can add here
   std::vector< Class > classes;
   ClassesRegistry& classesReg = getClassesRegistry();
   classesReg.getClassesMatchingType< Entity >( classes );

   if ( !classes.empty() )
   {
      QMenu* addMenu = popupMenu->addMenu( "Add" );

      unsigned int count = classes.size();
      for ( unsigned int i = 0; i < count; ++i )
      {
         entityEditor->createAddEntityAction( *addMenu, classes[i] );
      }
   }

   // add additional actions
   if ( parentEditor && item->getEntity() )
   {
      parentEditor->createRemoveEntityAction( *popupMenu, item->getEntity() );
   }
   entityEditor->createClearAction( *popupMenu ) ;

   // display the menu
   popupMenu->popup( m_sceneTree->mapToGlobal( pos ) );
}

///////////////////////////////////////////////////////////////////////////////

SceneTreeEditor* SceneTreeViewer::createEditor( EntityTreeItem* item )
{
   const Filesystem& fs = m_mgr->requestService< ResourcesManager >().getFilesystem();
   SceneTreeEditor* editor = NULL;

   if ( item )
   {
      Entity* entity = item->getEntity();
      if ( entity )
      {
         editor = new EntityEditor( fs, *entity );
      }
      else
      {
         editor = new ModelEditor( fs, *m_observedScene );
      }
   }
   else
   {
      editor = new ModelEditor( fs, *m_observedScene );
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

SceneTreeViewer::EntityTreeItem::EntityTreeItem( Entity* entity, QTreeWidgetItem* parent, TamyEditor& mgr )
: QTreeWidgetItem( parent )
, m_entity( entity )
{
   ASSERT( entity != NULL, "Entity can't be NULL" );

   setText( 0, getEntityName( entity ) );
   setText( 1, entity->getVirtualClass().getName().c_str() );

   setIcon( 0, getEntityIcon( entity, mgr ) );
}

///////////////////////////////////////////////////////////////////////////////

QString SceneTreeViewer::EntityTreeItem::getEntityName( Entity* entity ) const
{
   SpatialEntity* spatial;
   ShaderEffect* shader;
   Geometry* geometry;

   if ( ( spatial = dynamic_cast< SpatialEntity* >( entity ) ) != NULL )
   {
      return spatial->getName().c_str();
   }
   else if ( ( shader = dynamic_cast< ShaderEffect* >( entity ) ) != NULL )
   {
      return shader->getShaderName().c_str();
   }
   else if ( ( geometry = dynamic_cast< Geometry* >( entity ) ) != NULL )
   {
      return geometry->getGeometryName().c_str();
   }
   else
   {
      return "<<anonymous>>";
   }
}

///////////////////////////////////////////////////////////////////////////////

QIcon SceneTreeViewer::EntityTreeItem::getEntityIcon( Entity* entity, TamyEditor& mgr ) const
{
   ResourcesManager& resMgr = mgr.requestService< ResourcesManager >();
   QString iconsDir = resMgr.getFilesystem().getShortcut( "editorIcons" ).c_str();

   if ( dynamic_cast< SpatialEntity* >( entity ) != NULL )
   {
      return QIcon( iconsDir + "/spatialEntityIcon.png" );
   }
   else if ( dynamic_cast< ShaderEffect* >( entity ) != NULL )
   {
      return QIcon( iconsDir + "/shaderEffectIcon.png" );
   }
   else if ( dynamic_cast< Geometry* >( entity ) != NULL )
   {
      return QIcon( iconsDir + "/geometryIcon.png" );
   }
   else
   {
      return QIcon( iconsDir + "/unknownTypeIcon.png" );
   }
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
