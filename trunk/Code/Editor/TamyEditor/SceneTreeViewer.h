/// @file   TamyEditor\SceneTreeViewer.h
/// @brief  widget visualizing the contents of a scene as a tree
#pragma once

#include "core-MVC\ModelView.h"
#include "TreeWidget.h"
#include <QTreeWidgetItem>
#include <QFrame>
#include "SelectionManagerListener.h"
#include "TypeDescFactory.h"


///////////////////////////////////////////////////////////////////////////////

class Entity;
class SelectionManager;
class Camera;
class SceneTreeEditor;
class TreeWidget;

///////////////////////////////////////////////////////////////////////////////

/**
 * Component visualizing the contents of a scene as a tree
 */
class SceneTreeViewer : public QFrame, public ModelView, public SelectionManagerListener
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
       *
       * @param parent
       */
      EntityTreeItem( QTreeWidget* parent );
      
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
   TreeWidget*                   m_sceneTree;
   EntityTreeItem*               m_rootItem;
   QString                       m_iconsDir;
   Camera*                       m_camera;

   TypeDescFactory< Entity >*    m_itemsFactory;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    */
   SceneTreeViewer( QWidget* parentWidget );
   ~SceneTreeViewer();

   /**
    * Once a camera is set, each time a node is selected,
    * this camera will focus on it.
    *
    * @param camera
    */
   void setCamera( Camera& camera );

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
   void onEntitySelected( Entity& entity );
   void onEntityDeselected( Entity& entity );

signals:
   /**
    * Emitted when an entity in the tree is selected.
    *
    * @param entity
    */
   void onSceneTreeObjectSelected( Entity& entity );

   /**
    * Emitted when the selection changes so that no entity is selected any longer.
    */
   void onSceneTreeSelectionCleaned();

public slots:
   void selectItem( QTreeWidgetItem* item, int column );
   void focusOnItem( QTreeWidgetItem* item, int column );
   void getItemsFactory( QTreeWidgetItem* parent, TreeWidgetDescFactory*& outFactoryPtr );
   void addNode( QTreeWidgetItem* parent, unsigned int typeIdx );
   void removeNode( QTreeWidgetItem* parent, QTreeWidgetItem* child );
   void clearNode( QTreeWidgetItem* node );

private:
   void initUI();
   void buildEntitiesStack( Entity& entity, std::list< Entity* >& stack ) const;
   EntityTreeItem* find( Entity& entity );
   SceneTreeEditor* createEditor( EntityTreeItem* item );
};

///////////////////////////////////////////////////////////////////////////////
