#include "GraphLayout.h"
#include "core.h"
#include "GraphBlock.h"
#include <algorithm>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>


///////////////////////////////////////////////////////////////////////////////

GraphLayout::GraphLayout()
   : m_sourceSocket( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

GraphLayout::~GraphLayout()
{
   // the blocks are managed by an outside resource
   m_blocks.clear();

   m_sourceSocket = NULL;
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

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::startNegotiatingConnection( GraphBlockSocket& sourceSocket )
{
   m_sourceSocket = &sourceSocket;
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::finishNegotiatingConnection( GraphBlockSocket& destinationSocket )
{
   if ( m_sourceSocket == NULL )
   {
      // there was no source socket memorized to begin with - drop the whole thing
      return;
   }

   // verify that we're trying to connect two distinct sockets
   if ( m_sourceSocket == &destinationSocket )
   {
      return;
   }

   // create a connection
   new GraphBlockConnection( m_sourceSocket, &destinationSocket );

   // cleanup
   m_sourceSocket = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::restoreState()
{
   for ( std::vector< GraphBlock* >::iterator it = m_blocks.begin(); it != m_blocks.end(); ++it )
   {
      addItem( *it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::drawForeground( QPainter* painter, const QRectF& rect )
{
   __super::drawForeground( painter, rect );

   if ( m_sourceSocket != NULL )
   {
      // we're in the process of connecting two blocks together - visualize that

      painter->setRenderHint( QPainter::Antialiasing, true );
      painter->setPen( QPen( QBrush( QColor( 0, 0, 0 ) ), 3.0f ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::createContextMenu( QMenu* menu, const QPointF& scenePos )
{
   QGraphicsItem* pointedBlock = itemAt( scenePos );
   if ( pointedBlock == NULL )
   {
      // we clicked an empty spot - show a menu for adding new nodes
      QMenu* addMenu = menu->addMenu( "Add" );

      std::vector< Class > classes;
      getNodesClasses( classes );
      for( std::vector< Class >::const_iterator it = classes.begin(); it != classes.end(); ++it )
      {
         const Class& nodeClass = *it;

         QAction* addNodeAction = new GraphNodeCreationAction( *this, nodeClass, scenePos );
         addMenu->addAction( addNodeAction );
      }
   }
   
   // if there are blocks selected, show additional options
   QList< QGraphicsItem* > items = selectedItems();
   if ( items.isEmpty() == false )
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
   : QAction( "Remove", &parent )
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
