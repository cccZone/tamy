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
   QGraphicsScene*                     m_scene;
   std::vector< GraphBlock* >          m_blocks;
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
    */
   GraphWidget( QWidget* parent );
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
    * Called when we want to add a node to the graph.
    *
    * @param nodeClass
    */
   void addNode( void* pNode );

   /**
    * Called when we are removing a node from the graph.
    *
    * @param block
    */
   void removeNode( void* pNode );

   /**
    * Called when a popup menu is displayed.
    *
    * @param menu
    */
   void popupMenuShown( QMenu& menu );

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
   GraphBlock* createBlock( const Class& nodeType, void* node );
   NodeAssociacion* findAssociacion( const Class& nodeType );
   GraphBlock* findBlock( const QPointF& scenePos );
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
