/// @file   TamyEditor/GraphLayout.h
/// @brief  graph layout interface
#pragma once

#include <vector>
#include <QPointF>
#include "GraphBlock.h"
#include <QGraphicsScene>
#include <QAction>


///////////////////////////////////////////////////////////////////////////////

class QGraphicsLineItem;

///////////////////////////////////////////////////////////////////////////////

/**
 * An abstract graph layout interface. It stores a list of blocks, however those
 * are managed by a solid class implementing this interface.
 * The class should be a resource that will store the layout data and provide
 * a link with a model resource the blocks represent.
 */
class GraphLayout : public QGraphicsScene
{
   Q_OBJECT

protected:
   std::vector< GraphBlock* >                m_blocks;            // the blocks will be managed by an outside resource
   std::vector< GraphBlockConnection* >      m_connections;       // connections between the blocks

   GraphBlockSocket*                         m_sourceSocket;      // memorized graph socket instance where a newly created connection starts at
   QGraphicsLineItem*                        m_drawnConnection;

public:
   virtual ~GraphLayout();

   /**
    * Adds a new block to the layout.
    *
    * @param block
    */
   GraphBlock& add( const SerializableReflectionType& type, const QPointF& pos );

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
   void finishNegotiatingConnection( GraphBlockSocket* destinationSocket );

   /**
    * Removes the specified connection.
    *
    * @param connection
    */
   void removeConnection( GraphBlockConnection& connection );

signals:
   /**
    * Emitted when a block is added to the layout.
    */
   void onBlockAdded();

   /**
    * Emitted when a block is removed from the layout.
    */
   void onBlockRemoved();

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
   virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent );

   // -------------------------------------------------------------------------
   // Representations factory
   // -------------------------------------------------------------------------
   /**
    * Called when the widget creates a new representation for the specified node type.
    *
    * @param type    node type
    */
   virtual GraphBlock* createNode( const SerializableReflectionType& type ) = 0;

   /**
    * Called when the widget removes a node.
    *
    * @param node
    */
   virtual void removeNode( ReflectionObject& node ) = 0;

   /**
    * Returns a list of types we can select from when creating a new node.
    *
    * @param classes          fill this list with available node types
    */
   virtual void getNodesClasses( std::vector< const SerializableReflectionType* >& classes ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class GraphNodeCreationAction : public QAction
{
   Q_OBJECT

private:
   GraphLayout&                        m_parent;
   const SerializableReflectionType&   m_type;
   QPointF                             m_pos;

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param type        type of the node added
    * @param pos         position in graph onto which the node should be added
    */
   GraphNodeCreationAction( GraphLayout& parent, const SerializableReflectionType& type, const QPointF& pos );

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

class GraphSocketRemoveConnectionsAction : public QAction
{
   Q_OBJECT

private:
   GraphLayout&               m_parent;
   GraphBlockSocket&          m_socket;

public:
   /**
    * Constructor.
    *
    * @param socket
    */
   GraphSocketRemoveConnectionsAction( GraphLayout& parent, GraphBlockSocket& socket );

public slots:
   void onTriggered();
};

///////////////////////////////////////////////////////////////////////////////
