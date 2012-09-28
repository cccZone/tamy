#include "GraphLayout.h"
#include "core.h"
#include "GraphBlock.h"
#include "GraphBlockSocket.h"
#include <algorithm>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QGraphicsLineItem>


///////////////////////////////////////////////////////////////////////////////

GraphLayout::GraphLayout()
   : m_sourceSocket( NULL )
   , m_drawnConnection( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

GraphLayout::~GraphLayout()
{
   // the blocks are managed by an outside resource
   m_blocks.clear();

   m_sourceSocket = NULL;
   m_drawnConnection = NULL;
}

///////////////////////////////////////////////////////////////////////////////

GraphBlock& GraphLayout::add( const SerializableReflectionType& type, const QPointF& pos )
{
   GraphBlock* block = createNode( type );
   ASSERT_MSG( block != NULL, "No representation ready for this type of node" );
  
   m_blocks.push_back( block );

   // add the block to the scene
   addItem( block );
   block->setPos( pos );

   emit onBlockAdded();

   return *block;
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::remove( GraphBlock* block )
{
   ASSERT_MSG( block != NULL, "NULL blocks can't be removed from the layout" );
   if ( block == NULL )
   {
      return;
   }

   // remove all connections the block's sockets is involved in
   {
      std::vector< GraphBlockConnection* > connections;
      int count = m_connections.size();
      for ( int i = count - 1; i >= 0; --i )
      {
         GraphBlockConnection* connection = m_connections[i];
         if ( connection->getSource().getParentBlock() == block || connection->getDestination().getParentBlock() == block )
         {
            removeItem( connection );
            delete connection;
            m_connections.erase( m_connections.begin() + i );
         }
      }

   }

   // remove the block and the node it represents 
   {
      removeNode( *block->getNode() );

      std::vector< GraphBlock* >::iterator it = std::find( m_blocks.begin(), m_blocks.end(), block );
      if ( it != m_blocks.end() )
      {
         m_blocks.erase( it );
         removeItem( block );
      }
      else
      {
         ASSERT_MSG( false, "The specified block doesn't belong to this layout" );
      }
   }

   emit onBlockRemoved();
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::startNegotiatingConnection( GraphBlockSocket& sourceSocket )
{
   // we can only start connections from the sockets on the right
   if ( sourceSocket.getPosition() == GBSP_OUTPUT )
   {
      m_sourceSocket = &sourceSocket;

      // visualize the drawn connection
      ASSERT( m_drawnConnection == NULL);
      m_drawnConnection = new QGraphicsLineItem();
      addItem( m_drawnConnection );
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::finishNegotiatingConnection( GraphBlockSocket* destinationSocket )
{
   if ( m_drawnConnection )
   {
      // remove the visualization of the drawn connection
      removeItem( m_drawnConnection );
      delete m_drawnConnection;
      m_drawnConnection = NULL;
   }

   if ( m_sourceSocket == NULL || destinationSocket == NULL )
   {
      // there was no source socket memorized to begin with - drop the whole thing
      return;
   }

   // verify that we're trying to connect two distinct sockets that belong to two distinct blocks
   bool belongToTheSameBlock = ( m_sourceSocket->parentItem() == destinationSocket->parentItem() );
   if ( m_sourceSocket == destinationSocket || belongToTheSameBlock )
   {
      return;
   }

   // verify that the connection doesn't exist and that we're connecting to a correctly positioned socket
   if ( destinationSocket->getPosition() == GBSP_INPUT && !areSocketsConnected( m_sourceSocket, destinationSocket ) )
   {
      ReflectionObject* sourceNode = m_sourceSocket->getParentBlock()->getNode();
      ReflectionObject* destinationNode = destinationSocket->getParentBlock()->getNode();
      bool connectionEstablished = connectNodes( sourceNode, m_sourceSocket->getName(), destinationNode, destinationSocket->getName() );
      
      if ( connectionEstablished )
      {
         GraphBlockConnection* connection = new GraphBlockConnection( m_sourceSocket, destinationSocket );
         m_connections.push_back( connection );
         addItem( connection );
      }
   }

   // cleanup
   m_sourceSocket = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool GraphLayout::areSocketsConnected( GraphBlockSocket* source, GraphBlockSocket* destination ) const
{
   GraphBlock* sourceBlock = source->getParentBlock();
   GraphBlock* destinationBlock = destination->getParentBlock();

   uint count = m_connections.size();
   for ( uint i = 0; i < count; ++i )
   {
      GraphBlockConnection* connection = m_connections[i];
      if ( connection->doesConnect( sourceBlock, destinationBlock ) )
      {
         return true;
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::disconnectSocket( GraphBlockSocket& socket )
{
   ReflectionObject* node = socket.getParentBlock()->getNode();
   const std::string& socketName = socket.getName();

   // find and remove all connections the socket was involved int
   std::vector< GraphBlockConnection* > connectionsToDelete;

   int count = m_connections.size();
   for ( int i = count - 1; i >= 0; --i )
   {
      GraphBlockConnection* connection = m_connections[i];

      if ( &connection->getSource() == &socket || &connection->getDestination() == &socket )
      {
         connectionsToDelete.push_back( connection );

         m_connections.erase( m_connections.begin() + i );
      }
   }

   // first - run the connections through the higher instance that's aware of the pipeline
   // this graph represents, so that it can clean them up
   breakPipelineConnections( connectionsToDelete );

   // and then delete the connections
   count = connectionsToDelete.size();
   for ( int i = 0 ; i < count; ++i )
   {
      GraphBlockConnection* connection = connectionsToDelete[i];
      removeItem( connection );
      delete connection;
   }

}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
   __super::mouseMoveEvent( mouseEvent );

   if ( m_drawnConnection )
   {
      // update drawn connection
      ASSERT( m_sourceSocket != NULL );

      QPointF start = m_sourceSocket->scenePos();
      QPointF end = mouseEvent->scenePos();
      m_drawnConnection->setLine( start.rx(), start.ry(), end.rx(), end.ry() );
   }

}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::onBlockMoved()
{
   for( std::vector< GraphBlockConnection* >::iterator it = m_connections.begin(); it != m_connections.end(); ++it )
   {
      (*it)->calculateBounds();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::restoreState()
{
   // restore blocks
   for ( std::vector< GraphBlock* >::iterator it = m_blocks.begin(); it != m_blocks.end(); ++it )
   {
      GraphBlock* block = *it;
      addItem( block );

      // calculate the bounds of the block and its sockets so that they can be properly displayed on the canvas
      block->calculateBounds();

      const std::vector< GraphBlockSocket* >& sockets = block->accessSockets();
      uint socketsCount = sockets.size();
      for ( uint i = 0; i < socketsCount; ++i )
      {
         sockets[i]->calculateBounds();
      }
   }

   // remove connections to non-existing blocks
   int count = m_connections.size();
   for( int i = count - 1; i >= 0; --i )
   {
      if ( !m_connections[i]->isOk() )
      {
         GraphBlockConnection* connection = m_connections[i];
         removeItem( connection );
         delete connection;
         m_connections.erase( m_connections.begin() + i );
      }
   }

   // restore connections
   for ( std::vector< GraphBlockConnection* >::iterator it = m_connections.begin(); it != m_connections.end(); ++it )
   {
      addItem( *it );

      // calculate the bounds of the connections between the blocks so that they can be properly displayed on the canvas
      (*it)->calculateBounds();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::createContextMenu( QMenu* menu, const QPointF& scenePos )
{
   QGraphicsItem* pointedItem = itemAt( scenePos );
   GraphBlock* block = dynamic_cast< GraphBlock* >( pointedItem );
   GraphBlockSocket* socket = dynamic_cast< GraphBlockSocket* >( pointedItem );

   if ( pointedItem == NULL )
   {
      // we clicked an empty spot - show a menu for adding new nodes
      QMenu* addMenu = menu->addMenu( "Add nodes" );

      std::vector< const SerializableReflectionType* > classes;
      getNodesClasses( classes );
      for( std::vector< const SerializableReflectionType* >::const_iterator it = classes.begin(); it != classes.end(); ++it )
      {
         const SerializableReflectionType& nodeClass = **it;

         QAction* addNodeAction = new GraphNodeCreationAction( *this, nodeClass, scenePos );
         addMenu->addAction( addNodeAction );
      }
   }
   else if ( block != NULL )
   {
      // if the clicked item is a block...
      block->onCreateContextMenu( menu );
   }
   else if ( socket != NULL )
   {
      // if the clicked item is a socket...
      QAction* removeConnectionsAction = new GraphSocketRemoveConnectionsAction( *this, *socket );
      menu->addAction( removeConnectionsAction );
   }
   
   // if there are blocks selected, show additional options
   QList< QGraphicsItem* > items = selectedItems();
   QList< GraphBlock* > selectedBlocks;
   foreach( QGraphicsItem* item, items )
   {
      GraphBlock* block = dynamic_cast< GraphBlock* >( item );
      if ( block )
      {
         selectedBlocks.push_back( block );
      }
   }
   if ( !selectedBlocks.isEmpty() )
   {
      // an option for removing existing nodes
      QAction* removeNodesAction = new GraphNodeRemoveAction( *this, items );
      menu->addAction( removeNodesAction );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GraphNodeCreationAction::GraphNodeCreationAction( GraphLayout& parent, const SerializableReflectionType& type, const QPointF& pos )
   : QAction( type.m_name.c_str(), &parent )
   , m_parent( parent )
   , m_type( type )
   , m_pos( pos )
{
   connect( this, SIGNAL( triggered() ), this, SLOT( onTriggered() ) );
}

///////////////////////////////////////////////////////////////////////////////

void GraphNodeCreationAction::onTriggered()
{
   m_parent.add( m_type, m_pos );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GraphNodeRemoveAction::GraphNodeRemoveAction( GraphLayout& parent, const QList< QGraphicsItem* >& items )
   : QAction( "Remove nodes", &parent )
   , m_parent( parent )
   , m_items( items )
{
   connect( this, SIGNAL( triggered() ), this, SLOT( onTriggered() ) );
}

///////////////////////////////////////////////////////////////////////////////

void GraphNodeRemoveAction::onTriggered()
{
   foreach( QGraphicsItem* item, m_items )
   {
      m_parent.remove( dynamic_cast< GraphBlock* >( item ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GraphSocketRemoveConnectionsAction::GraphSocketRemoveConnectionsAction( GraphLayout& parent, GraphBlockSocket& socket )
   : QAction( "Remove all connections", &parent )
   , m_parent( parent )
   , m_socket( socket )
{
   connect( this, SIGNAL( triggered() ), this, SLOT( onTriggered() ) );
}

///////////////////////////////////////////////////////////////////////////////

void GraphSocketRemoveConnectionsAction::onTriggered()
{
   m_parent.disconnectSocket( m_socket );
}

///////////////////////////////////////////////////////////////////////////////
