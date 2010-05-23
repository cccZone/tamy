#pragma once

/// @file   TamyEditor\TreeWidget.h
/// @brief  A tree widget with some basic common functionality

#include <QTreeWidget.h>
#include <QAction.h>


///////////////////////////////////////////////////////////////////////////////

class QMenu;
class QPoint;
class QTreeWidgetItem;
class TypeSelectionAction;
class TreeWidgetDescFactory;

///////////////////////////////////////////////////////////////////////////////

/**
 * A tree widget with some basic common functionality.
 */
class TreeWidget : public QTreeWidget
{
   Q_OBJECT

private:
   QString                 m_iconsDir;
   QTreeWidgetItem*        m_popupMenuItem;

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param objName       object name
    * @param iconsDir      a directory containing editor icons
    */
   TreeWidget( QWidget* parent, const QString& objName, const QString& iconsDir );

   /**
    * Gives access to an item for which a popup menu was created.
    */
   inline QTreeWidgetItem* getPopupMenuItem() { return m_popupMenuItem; }

signals:
   /**
    * Acquires an instance of items factory, items which can be added
    * to the specified parent item.
    *
    * @param parent
    * @param outFactoryPtr
    */
   void getItemsFactory( QTreeWidgetItem* parent, TreeWidgetDescFactory*& outFactoryPtr );

   /**
    * Called when we want to add a child node to the specified parent node.
    * The child's type is specified with a typeIdx.
    *
    * @param parent
    * @param typeIdx
    */
   void addNode( QTreeWidgetItem* parent, unsigned int typeIdx );

   /**
    * Removes a child node form a parent node
    *
    * @param parent
    * @param child
    */
   void removeNode( QTreeWidgetItem* parent, QTreeWidgetItem* child );

   /**
    * Removes all children from the specified node
    *
    * @param node
    */
   void clearNode( QTreeWidgetItem* node );

   /**
    * Called when a popup menu is displayed.
    *
    * @param parent
    * @param menu
    */
   void popupMenuShown( QTreeWidgetItem* parent, QMenu& menu );

public slots:
   void showPopupMenu( const QPoint& pos );
   void expandNode();
   void collapseNode();
   void removeNode();
   void clearNode();

private:
   friend class NodeCreationAction;
   void addNode( int typeIdx );

   void deepCollapse( QTreeWidgetItem* root, bool expand );
};

///////////////////////////////////////////////////////////////////////////////

class NodeCreationAction : public QAction
{
   Q_OBJECT

private:
   TreeWidget&    m_parent;
   int            m_typeIdx;

public:
   /**
    * Constructor.
    *
    * @param icon
    * @param desc
    * @param typeIdx
    * @param parent
    */
   NodeCreationAction( const QIcon& icon, 
                       const QString& desc,
                       unsigned int typeIdx,
                       TreeWidget* parent );

public slots:
   void onTriggered();
};

///////////////////////////////////////////////////////////////////////////////
