#pragma once

/// @file   TamyEditor\SceneTreeViewer.h
/// @brief  component visualizing the contents of a scene as a tree

#include "core\Component.h"
#include "core-MVC\ModelView.h"
#include "TreeWidget.h"
#include <QTreeWidgetItem>
#include <QObject>
#include "SelectionManager.h"
#include "TypeDescFactory.h"


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class Entity;
class SelectionManager;
class Camera;
class SceneTreeEditor;
class TreeWidget;

///////////////////////////////////////////////////////////////////////////////

/**
 * Component visualizing the contents of a scene as a tree
 */
class SceneTreeViewer : public QObject,
                        public Component< TamyEditor >,
                        public ModelView,
                        public SelectionManagerListener
{
   Q_OBJECT

private:
   /**
    * An item describing a scene entity in the tree structure
    */
   class EntityTreeItem : public QTreeWidgetItem 
   {
   private:
      Entity*     m_entity;

   public:
      /**
       * Constructor for the topmost 'world' entry.
       */
      EntityTreeItem( QTreeWidget* parent, TamyEditor& mgr );
      
      /**
       * Constructor for the regular entity-related entires.
       */
      EntityTreeItem( Entity* entity, QTreeWidgetItem* parent, TypeDescFactory< Entity >& itemsFactory );

      /**
       * Updates the item's description.
       */
      void update();

      /**
       * Returns the entity stored under the entry.
       */
      inline Entity* getEntity() { return m_entity; }

      /**
       * Returns the entity stored under the entry (const version).
       */
      inline const Entity* getEntity() const { return m_entity; }

      /**
       * Searches for a child entry matching the specified entity.
       *
       * @param entity
       */
      EntityTreeItem* find( Entity* entity );

   private:
      QString getEntityName( Entity* entity ) const;
   };

private:
   TamyEditor*                   m_mgr;
   TreeWidget*                   m_sceneTree;
   EntityTreeItem*               m_rootItem;

   QString                       m_iconsDir;

   Model*                        m_observedScene;
   SelectionManager*             m_selectionMgr;
   Camera*                       m_camera;

   TypeDescFactory< Entity >*    m_itemsFactory;

public:
   /**
    * Constructor.
    */
   SceneTreeViewer();
   ~SceneTreeViewer();

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );
   void onServiceRegistered( TamyEditor& mgr );

   // -------------------------------------------------------------------------
   // ModelView implementation
   // -------------------------------------------------------------------------
   void onEntityAdded( Entity& entity );
   void onEntityRemoved( Entity& entity );
   void onEntityChanged( Entity& entity );
   void resetContents();

   // -------------------------------------------------------------------------
   // SelectionManagerListener implementation
   // -------------------------------------------------------------------------
   void onObjectSelected( Entity& entity );
   void onObjectDeselected( Entity& entity );

public slots:
   void selectItem( QTreeWidgetItem* item, int column );
   void focusOnItem( QTreeWidgetItem* item, int column );
   void getItemsFactory( QTreeWidgetItem* parent, TreeWidgetDescFactory*& outFactoryPtr );
   void addNode( QTreeWidgetItem* parent, unsigned int typeIdx );
   void removeNode( QTreeWidgetItem* parent, QTreeWidgetItem* child );
   void clearNode( QTreeWidgetItem* node );

private:
   void initUI( TamyEditor& mgr );
   void buildEntitiesStack( Entity& entity, std::list< Entity* >& stack ) const;
   EntityTreeItem* find( Entity& entity );
   SceneTreeEditor* createEditor( EntityTreeItem* item );
};

///////////////////////////////////////////////////////////////////////////////
