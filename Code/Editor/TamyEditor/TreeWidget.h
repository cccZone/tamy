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
    * Called when a pop-up menu is displayed.
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
   void deepCollapse( QTreeWidgetItem* root, bool expand );
};

///////////////////////////////////////////////////////////////////////////////
