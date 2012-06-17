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

SceneTreeViewer::SceneTreeViewer( QWidget* parentWidget )
   : QFrame( parentWidget )
   , m_sceneTree( NULL )
   , m_rootItem( NULL )
   , m_itemsFactory( NULL )
   , m_camera( NULL )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   const Filesystem& fs = resMgr.getFilesystem();
   m_iconsDir = fs.getShortcut( "editorIcons" ).c_str();

   m_itemsFactory = new TypeDescFactory< Entity >( m_iconsDir, fs, "unknownTypeIcon.png" );

   initUI();
}

///////////////////////////////////////////////////////////////////////////////

SceneTreeViewer::~SceneTreeViewer()
{
   m_sceneTree = NULL;
   m_rootItem = NULL;

   delete m_itemsFactory;
   m_itemsFactory = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::initUI()
{
   QVBoxLayout* layout = new QVBoxLayout( this );
   setLayout( layout );
   layout->setSpacing(0);
   layout->setMargin(0);

   // setup the scene tree container widget
   m_sceneTree = new TreeWidget( this, "SceneTreeViewer/m_sceneTree", m_iconsDir );
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

   m_rootItem = new EntityTreeItem( m_sceneTree );
   m_sceneTree->addTopLevelItem( m_rootItem );
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::setCamera( Camera& camera )
{
   m_camera = &camera;
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::onEntityAdded( Entity& entity )
{
   if ( !m_rootItem )
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
   EntityTreeItem* entityItem = dynamic_cast< EntityTreeItem* >( item );
   if ( !entityItem )
   {
      return;
   }

   if ( entityItem->getEntity() )
   {
      emit onSceneTreeObjectSelected( *entityItem->getEntity() );
   }
   else
   {
      emit onSceneTreeSelectionCleaned();
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
   SceneTreeEditor* editor = createEditor( dynamic_cast< EntityTreeItem* >( parent ) );
   editor->addEntity( m_itemsFactory->getClass( typeIdx ) );
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::removeNode( QTreeWidgetItem* parent, QTreeWidgetItem* child )
{
   SceneTreeEditor* parentEditor = createEditor( dynamic_cast< EntityTreeItem* >( parent ) );
   EntityTreeItem* item = dynamic_cast< EntityTreeItem* >( child );
   parentEditor->removeEntity( item->getEntity() );
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::clearNode( QTreeWidgetItem* node )
{
   SceneTreeEditor* editor = createEditor( dynamic_cast< EntityTreeItem* >( node ) );
   editor->clearEntity();
}

///////////////////////////////////////////////////////////////////////////////

SceneTreeEditor* SceneTreeViewer::createEditor( EntityTreeItem* item )
{
   const std::vector< Model* >& observedModels = getObservedModels();
   if ( observedModels.empty() )
   {
      return NULL;
   }

   // we're only operating on one model here
   Model* observedModel = getObservedModels()[0];
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
         editor = new ModelEditor( *observedModel );
      }
   }
   else
   {
      editor = new ModelEditor( *observedModel );
   }

   return editor;
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::onEntitySelected( Entity& entity )
{
   EntityTreeItem* entityItem = find( entity );
   {
      m_sceneTree->setItemSelected( entityItem, true );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneTreeViewer::onEntityDeselected( Entity& entity )
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

SceneTreeViewer::EntityTreeItem::EntityTreeItem( QTreeWidget* parent )
: QTreeWidgetItem( parent )
, m_entity( NULL )
{
   setText( 0, "World" );
   setText( 1, "Model" );

   ResourcesManager& resMgr = ResourcesManager::getInstance();
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
   const SerializableReflectionType& virtType = entity->getVirtualRTTI();
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
      return geometry->getMeshName().c_str();
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
