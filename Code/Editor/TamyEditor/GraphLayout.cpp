#include "GraphLayout.h"
#include "core.h"
#include "GraphBlock.h"
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

GraphBlock& GraphLayout::add( const Class& type, const QPointF& pos )
{
   GraphBlock* block = createNode( type );
   ASSERT_MSG( block != NULL, "No representation ready for this type of node" );
  
   m_blocks.push_back( block );

   // add the block to the scene
   addItem( block );
   block->setPos( pos );

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
   std::vector< GraphBlockConnection* > connections;
   block->getConnections( connections );
   for ( std::vector< GraphBlockConnection* >::iterator it = connections.begin(); it != connections.end(); ++it )
   {
      removeConnection( **it );
   }

   // remove the block itself
   removeNode( block->getNode() );

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
   if ( destinationSocket->getPosition() == GBSP_INPUT && !m_sourceSocket->isConnectedTo( *destinationSocket ) )
   {
      // create a connection
      GraphBlockConnection* connection = GraphBlockConnection::createConnection( m_sourceSocket, destinationSocket );
      if ( connection )
      {
         m_connections.push_back( connection );
         addItem( connection );
      }
   }

   // cleanup
   m_sourceSocket = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::removeConnection( GraphBlockConnection& connection )
{
   for( std::vector< GraphBlockConnection* >::iterator it = m_connections.begin(); it != m_connections.end(); ++it )
   {
      if ( *it == &connection )
      {
         connection.onRemoved();
         
         removeItem( *it );
         delete *it;
         m_connections.erase( it );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
   __super::mouseMoveEvent( mouseEvent );

   if ( !m_drawnConnection )
   {
      return;
   }
   ASSERT( m_sourceSocket != NULL );

   QPointF start = m_sourceSocket->scenePos();
   QPointF end = mouseEvent->scenePos();
   m_drawnConnection->setLine( start.rx(), start.ry(), end.rx(), end.ry() );
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::restoreState()
{
   // restore blocks
   for ( std::vector< GraphBlock* >::iterator it = m_blocks.begin(); it != m_blocks.end(); ++it )
   {
      addItem( *it );
   }

   // restore connections
   for ( std::vector< GraphBlockConnection* >::iterator it = m_connections.begin(); it != m_connections.end(); ++it )
   {
      addItem( *it );
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

      std::vector< Class > classes;
      getNodesClasses( classes );
      for( std::vector< Class >::const_iterator it = classes.begin(); it != classes.end(); ++it )
      {
         const Class& nodeClass = *it;

         QAction* addNodeAction = new GraphNodeCreationAction( *this, nodeClass, scenePos );
         addMenu->addAction( addNodeAction );
      }
   }
   else if ( block != NULL )
   {
      // if the clicked item is a block...
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

GraphNodeCreationAction::GraphNodeCreationAction( GraphLayout& parent, const Class& type, const QPointF& pos )
   : QAction( type.getShortName().c_str(), &parent )
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
   std::vector< GraphBlockConnection* > connections = m_socket.getConnections();
   for ( std::vector< GraphBlockConnection* >::iterator it = connections.begin(); it != connections.end(); ++it )
   {
      m_parent.removeConnection( **it );
   }
}

///////////////////////////////////////////////////////////////////////////////
