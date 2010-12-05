/// @file   TamyEditor/GraphLayout.h
/// @brief  graph layout interface
#pragma once

#include <vector>
#include <QPointF>
#include "GraphBlock.h"
#include <QGraphicsScene>
#include <QAction>


///////////////////////////////////////////////////////////////////////////////

/**
 * An abstract graph layout interface. It stores a list of blocks, however those
 * are managed by a solid class implementing this interface.
 * The class should be a resource that will store the layout data and provide
 * a link with a model resource the blocks represent.
 */
class GraphLayout : public QGraphicsScene
{
protected:
   std::vector< GraphBlock* >          m_blocks;            // the blocks will be managed by an outside resource

   GraphBlockSocket*                   m_sourceSocket;      // memorized graph socket instance where a newly created connection starts at

public:
   virtual ~GraphLayout();

   /**
    * Adds a new block to the layout.
    *
    * @param block
    */
   GraphBlock& add( const Class& type, const QPointF& pos );

   /**
    * Removes a block from the layout.
    *
    * @param block
    */
   void remove( GraphBlock* block );

   /**
    * Creates a context menu for the graph.
    *
    * @param menu       menu to fill in with entries
    * @param scenePos   position on the scene clicked
    */
   void createContextMenu( QMenu* menu, const QPointF& scenePos );

   /**
    * Opens the process of connecting two blocks through their sockets.
    *
    * @param sourceSocket
    */
   void startNegotiatingConnection( GraphBlockSocket& sourceSocket );

   /**
    * Finishes the process of connecting two blocks through their sockets.
    *
    * @param destinationSocket
    */
   void finishNegotiatingConnection( GraphBlockSocket& destinationSocket );
   
protected:
   /**
    * Default constructor.
    *
    * @param path
    */
   GraphLayout();

   /**
    * Called after the layout is loaded in order to re-add the deserialized blocks to the graphics scene
    */
   void restoreState();

   // -------------------------------------------------------------------------
   // QGraphicsScene implementation
   // -------------------------------------------------------------------------
   void drawForeground( QPainter* painter, const QRectF& rect );

   // -------------------------------------------------------------------------
   // Representations factory
   // -------------------------------------------------------------------------
   /**
    * Called when the widget creates a new representation for the specified node type.
    *
    * @param type    node type
    */
   virtual GraphBlock* createNode( const Class& type ) = 0;

   /**
    * Called when the widget removes a node.
    *
    * @param node
    */
   virtual void removeNode( Object& node ) = 0;

   /**
    * Returns a list of types we can select from when creating a new node.
    *
    * @param classes          fill this list with available node types
    */
   virtual void getNodesClasses( std::vector< Class >& classes ) = 0;  
};

///////////////////////////////////////////////////////////////////////////////

class GraphNodeCreationAction : public QAction
{
   Q_OBJECT

private:
   GraphLayout&   m_parent;
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
   GraphNodeCreationAction( GraphLayout& parent, const Class& type, const QPointF& pos );

public slots:
   void onTriggered();
};

///////////////////////////////////////////////////////////////////////////////

class GraphNodeRemoveAction : public QAction
{
   Q_OBJECT

private:
   GraphLayout&               m_parent;
   QList< QGraphicsItem* >    m_items;

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param items       items to remove
    */
   GraphNodeRemoveAction( GraphLayout& parent, const QList< QGraphicsItem* >& items );

public slots:
   void onTriggered();
};

///////////////////////////////////////////////////////////////////////////////
