/// @file      TamyEditor/GraphWidget.h
/// @brief     a widget for displaying & editing graphs
#ifndef _GRAPH_WIDGET_H
#define _GRAPH_WIDGET_H

#include <QGraphicsView.h>
#include <QGraphicsScene.h>
#include <QAction>
#include <vector>
#include "core/Class.h"
#include "GraphBlock.h"


///////////////////////////////////////////////////////////////////////////////

class QMenu;
class QTimer;
class GraphLayout;

///////////////////////////////////////////////////////////////////////////////

/**
 * A widget for displaying & editing graphs.
 */
class GraphWidget : public QGraphicsView
{
   Q_OBJECT

private:
   enum SelectionMode
   {
      SM_EXCLUSIVE,
      SM_ADDITIVE
   };

private:
   GraphLayout&                        m_layout;

   SelectionMode                       m_selectionMode;

   QTimer*                             m_dragTimer;
   bool                                m_isDraggingBlocks;
   QPoint                              m_prevMousePos;

public:
   /**
    * Constructor.
    *
    * @param parent              parent widget
    * @param layout              layout to edit
    */
   GraphWidget( QWidget* parent, GraphLayout& layout );
   ~GraphWidget();

   /**
    * Adds a new node to the graph onto the specified position.
    *
    * @param nodeType
    * @param addingPos
    */
   void addNodeAction( const Class& nodeType, const QPointF& addingPos );

   /**
    * Removes an item form the graph and its visual representation.
    *
    * @param item       representation of a graph node
    */
   void removeNodeAction( QGraphicsItem* item );

public slots:
   void onDragTimerTimeout();
   void onSceneSelectionChanged();

signals:
   /**
    * Called when a popup menu is displayed.
    *
    * @param menu
    */
   void popupMenuShown( QMenu& menu );

   /**
    * Called when a block is selected.
    *
    * @param object        object enclosed in the selected block ( can be NULL - in that case a block was deselected )
    */
   void blockSelected( Object* object );

protected:
   // -------------------------------------------------------------------------
   // QGraphicsView implementation
   // -------------------------------------------------------------------------
   void mousePressEvent( QMouseEvent* event );
   void mouseReleaseEvent( QMouseEvent* event );
   void mouseMoveEvent( QMouseEvent* event );
   void keyPressEvent( QKeyEvent* event );
   void keyReleaseEvent( QKeyEvent* event );


private:
   void showPopupMenu( const QPoint& activationPoint );
   void handleBlockSelection( const QPointF& scenePos );
};

///////////////////////////////////////////////////////////////////////////////

class GraphNodeCreationAction : public QAction
{
   Q_OBJECT

private:
   GraphWidget&   m_parent;
   Class          m_type;
   QPointF        m_pos;

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param type        type of the node added
    * @param pos         position in graph onto which the node should be added
    */
   GraphNodeCreationAction( GraphWidget& parent, const Class& type, const QPointF& pos );

public slots:
   void onTriggered();
};

///////////////////////////////////////////////////////////////////////////////

class GraphNodeRemoveAction : public QAction
{
   Q_OBJECT

private:
   GraphWidget&               m_parent;
   QList< QGraphicsItem* >    m_items;

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param items       items to remove
    */
   GraphNodeRemoveAction( GraphWidget& parent, const QList< QGraphicsItem* >& items );

public slots:
   void onTriggered();
};

///////////////////////////////////////////////////////////////////////////////

#include "GraphWidget.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_WIDGET_H
