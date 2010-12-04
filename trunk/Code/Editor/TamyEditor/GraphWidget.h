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
   struct NodeAssociacion
   {
   private:
      Class                m_type;

   public:
      GraphBlock::Shape    m_shape;
      QColor               m_bgColor;

      NodeAssociacion( const Class& type ) : m_type( type ) {}
      bool operator==( const Class& type ) const { return m_type == type; }
   };

   enum SelectionMode
   {
      SM_EXCLUSIVE,
      SM_ADDITIVE
   };

private:
   GraphLayout&                        m_layout;

   std::vector< NodeAssociacion >      m_associacions;

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
    * Associates a specified node type with its representation's appearance
    */
   template< typename T >
   void associate( GraphBlock::Shape shape, const QColor& bgColor );

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
    * Acquires a list of applicable node classes.
    *
    * @param classes
    */
   void getNodesClasses( std::vector< Class >& classes );

   /**
    * Acquires a list of applicable edges classes.
    *
    * @param classes
    */
   void getEdgesClasses( std::vector< Class >& classes );

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
   GraphBlock* createBlock( const Class& nodeType, Object* node );
   NodeAssociacion* findAssociacion( const Class& nodeType );
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
