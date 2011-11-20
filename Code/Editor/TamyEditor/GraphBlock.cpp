#include "GraphBlock.h"
#include "GraphLayout.h"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>
#include "core/Algorithms.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( GraphBlock, Object );
   PROPERTY_EDIT( "caption", std::string, m_caption );
   PROPERTY( QPointF, m_position );
   PROPERTY( std::vector< GraphBlockSocket* >, m_sockets );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

QPen GraphBlock::s_textPen( QBrush( QColor( 0, 0, 0 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
QPen GraphBlock::s_borderPen( QBrush( QColor( 0, 0, 0 ) ), 3.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
QPen GraphBlock::s_selectionPen( QBrush( QColor( 255, 226, 96 ) ), 3.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

///////////////////////////////////////////////////////////////////////////////

GraphBlock::GraphBlock()
   : QGraphicsItem( NULL )
   , m_caption( "" )
   , m_bounds( QPointF( 0, 0 ), QSizeF( 100, 100 ) )
   , m_captionBounds( QPointF( 0, 0 ), QSizeF( 100, 0 ) )
   , m_font( "Arial", 15, QFont::Light )
{
   m_font.setStyle( QFont::StyleNormal );
   m_font.setStyleHint( QFont::AnyStyle );
   m_font.setStyleStrategy( QFont::PreferAntialias );

   setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable );

   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

GraphBlock::~GraphBlock()
{
}

///////////////////////////////////////////////////////////////////////////////

bool GraphBlock::doesOverlap( const QPointF& pos ) const
{
   QRectF bounds( pos, QSizeF( 1, 1 ) );
   return m_bounds.intersects( bounds );
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{ 
   Shape shape = getShape();
   QColor fillColor = getBgColor();
   QPen pen = getBorderPen();

   painter->save();

   painter->setRenderHint( QPainter::Antialiasing, true );
   painter->setRenderHint( QPainter::TextAntialiasing, true );
   painter->setFont( m_font );
   // draw the header
   {
      painter->setPen( pen );
      painter->setBrush( fillColor );

      switch( shape )
      {
      case GBS_RECTANGLE:
         {
            painter->drawRect( m_captionBounds );
            break;
         }

      case GBS_CIRCLE:
         {
            QPainterPath path;
            int halfHeight = ( m_captionBounds.bottom() + m_captionBounds.top() ) * 0.5;
            path.moveTo( m_captionBounds.right(), m_captionBounds.bottom() );
            path.lineTo( m_captionBounds.right(), halfHeight );
            path.arcTo( m_captionBounds, 0.0f, 180.0f );
            path.lineTo( m_captionBounds.left(), m_captionBounds.bottom() );

            painter->drawPath( path );

            break;
         }

      case GBS_ROUNDED:
         {
            painter->drawRoundedRect( m_captionBounds, 4.0f, 4.0f );
            break;
         }
      }

      painter->drawText( m_captionBounds, m_caption.c_str(), QTextOption( Qt::AlignCenter ) );
   }

   // draw the block's layout
   {
      painter->setPen( pen );
      painter->setBrush( QColor( 150, 150, 150 ) );
      painter->drawRect( m_bounds );
   }

   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
   __super::mouseMoveEvent( event );

   // inform the sockets about the move, so that they in turn can refresh 
   // the connections bounds
   for ( std::vector< GraphBlockSocket* >::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it )
   {
      (*it)->calculateConnectionBounds();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::onObjectPreSave()
{
   __super::onObjectPreSave();

   m_position = pos();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::onObjectLoaded()
{
   __super::onObjectLoaded();

   setPos( m_position );

   // recalculate the objects bounds
   calculateBounds();

   // inform the sockets about the move, so that they in turn can refresh 
   // the connections bounds
   for ( std::vector< GraphBlockSocket* >::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it )
   {
      (*it)->calculateConnectionBounds();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::onPropertyChanged( Property& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_caption" )
   {
      // recalculate the block bounds each time the caption changes
      calculateBounds();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::setCaption( const char* caption )
{
   m_caption = caption;
   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::addSocket( GraphBlockSocket* socket )
{
   if ( !socket )
   {
      return;
   }

   m_sockets.push_back( socket );
   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::removeSockets( GraphBlockSocketPosition position, const std::set< std::string >& socketNames )
{
   for ( std::set< std::string >::const_iterator it = socketNames.begin(); it != socketNames.end(); ++it )
   {
      unsigned int count = m_sockets.size();
      for ( unsigned int i = 0; i < count; ++i )
      {
         GraphBlockSocket* socket = m_sockets[i];
         if ( socket->getPosition() == position && socket->getName() == *it )
         {
            delete socket;
            m_sockets.erase( m_sockets.begin() + i );
            break;
         }
      }
   }

   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::calculateBounds()
{
   QFontMetrics metrics( m_font );
   float fontHeight = metrics.height();

   // calculate the size of the caption
   float captionWidth = metrics.width( m_caption.c_str() );
   float captionHeight = fontHeight + 6;

   // go through the sockets, include their width and calculate the required block height
   float longestLeftSocketName = 0;
   float longestRightSocketName = 0;
   unsigned int leftSocketsCount = 0;
   unsigned int rightSocketsCount = 0;
   for ( std::vector< GraphBlockSocket* >::const_iterator it = m_sockets.begin(); it != m_sockets.end(); ++it )
   {
      GraphBlockSocket* socket = *it;
      switch( socket->getPosition() )
      {
      case GBSP_INPUT:
         {
            longestLeftSocketName = max( longestLeftSocketName, socket->getNameWidth() );
            ++leftSocketsCount;
            break;
         }
      
      case GBSP_OUTPUT:
         {
            longestRightSocketName = max( longestRightSocketName, socket->getNameWidth() );
            ++rightSocketsCount;
            break;
         }
      }
   }
   float socketSize = metrics.height() + 6;
   float socketsHeight = max( leftSocketsCount, rightSocketsCount ) * socketSize;

   // calculate the bounds
   float blockWidth = max( captionWidth, longestLeftSocketName + longestRightSocketName );
   blockWidth = max( blockWidth, 100.0f ); // make sure the block isn't too narrow

   float blockHeight = captionHeight + socketsHeight;

   // set the caption bounds
   m_captionBounds.setWidth( blockWidth );
   m_captionBounds.setHeight( captionHeight );
   m_totalBounds = m_captionBounds;

   // set the block bounds
   m_bounds.setTop( captionHeight );
   m_bounds.setWidth( blockWidth );
   m_bounds.setHeight( blockHeight );
   m_totalBounds = m_totalBounds.united( m_bounds );

   // position the sockets
   float leftSocketsSpacing = ( leftSocketsCount > 0 ) ? ( socketsHeight / (float)leftSocketsCount ) : 0;
   float rightSocketsSpacing = ( rightSocketsCount > 0 ) ? ( socketsHeight / (float)rightSocketsCount ) : 0;

   float rightSocketY = rightSocketsSpacing * 0.5f;
   float leftSocketY = leftSocketsSpacing * 0.5f;
   float socketRectSize = 10.0f;
   for ( std::vector< GraphBlockSocket* >::const_iterator it = m_sockets.begin(); it != m_sockets.end(); ++it )
   {
      GraphBlockSocket* socket = *it;

      switch( socket->getPosition() )
      {
      case GBSP_INPUT:
         {
            socket->setPos( 0, captionHeight + leftSocketY );
            leftSocketY += leftSocketsSpacing;
            break;
         }

      case GBSP_OUTPUT:
         {
            socket->setPos( blockWidth, captionHeight + rightSocketY );
            rightSocketY += rightSocketsSpacing;
            break;
         }
      }
   }

   // include the size of the sockets in the block's bounding box
   m_totalBounds = QRectF( m_totalBounds.left() - 11, m_totalBounds.top(), m_totalBounds.right() + 21, m_totalBounds.bottom() ); 
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::getConnections( std::vector< GraphBlockConnection* >& outConnections ) const
{
   for ( std::vector< GraphBlockSocket* >::const_iterator it = m_sockets.begin(); it != m_sockets.end(); ++it )
   {
      GraphBlockSocket* socket = *it;
      outConnections.insert( outConnections.end(), socket->getConnections().begin(), socket->getConnections().end() );
   }
}

///////////////////////////////////////////////////////////////////////////////

GraphBlockSocket* GraphBlock::getSocket( GraphBlockSocketPosition position, const std::string& name ) const
{
   unsigned int count = m_sockets.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      GraphBlockSocket* socket = m_sockets[i];
      if ( socket->getPosition() == position && socket->getName() == name )
      {
         // found it
         return socket;
      }
   }

   // socket with such name doesn't exist
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::getAllSockets( GraphBlockSocketPosition position, std::set< std::string >& outSocketNames ) const
{
   unsigned int count = m_sockets.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      GraphBlockSocket* socket = m_sockets[i];
      if ( socket->getPosition() == position  )
      {
         outSocketNames.insert( socket->getName() );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GraphBlockSocket, Object );
   PROPERTY( GraphBlock*, m_parent );
   PROPERTY( QPointF, m_position );
   PROPERTY( GraphBlockSocketPosition, m_blockSide );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GraphBlockConnection* >, m_connections );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

QPen GraphBlockSocket::s_borderPen( QBrush( QColor( 0, 0, 0 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

///////////////////////////////////////////////////////////////////////////////

GraphBlockSocket::GraphBlockSocket()
   : QGraphicsItem( NULL )
   , m_parent( NULL )
   , m_blockSide( GBSP_INPUT )
   , m_name( "" )
   , m_font( "Arial", 10, QFont::Light )
{
   setFlag( QGraphicsItem::ItemIsSelectable, true );
   setFlag( QGraphicsItem::ItemIsMovable, false );

   setAcceptTouchEvents( true );
   setAcceptedMouseButtons( Qt::LeftButton );
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::onObjectPreSave()
{
   __super::onObjectPreSave();

   m_position = pos();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::onObjectLoaded()
{
   __super::onObjectLoaded();

   ASSERT_MSG( m_parent != NULL, "Information about this socket's parent was not deserialized properly" );
   setParentItem( m_parent );
   setPos( m_position );

   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::initialize( GraphBlock* parent, GraphBlockSocketPosition pos, const char* name )
{ 
   m_parent = parent;
   m_blockSide = pos;
   m_name = name;
   setParentItem( parent );

   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

float GraphBlockSocket::getNameWidth() const
{
   QFontMetrics metrics( m_font );
   return metrics.width( m_name.c_str() );
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
   painter->save();

   painter->setRenderHint( QPainter::Antialiasing, true );
   painter->setRenderHint( QPainter::TextAntialiasing, true );

   QPen borderPen = getBorderPen();
   QColor bgColor = getBgColor();

   painter->setPen( borderPen );
   painter->setBrush( bgColor );
   painter->drawRect( m_bounds );

   painter->setFont( m_font );
   painter->setBrush( QColor( 0, 0, 0 ) );
   painter->drawText( m_nameBounds, m_name.c_str(), QTextOption( Qt::AlignCenter ) );

   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::calculateConnectionBounds()
{
   for( std::vector< GraphBlockConnection* >::iterator it = m_connections.begin(); it != m_connections.end(); ++it )
   {
      (*it)->calculateBounds();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::calculateBounds()
{
   QFontMetrics metrics( m_font );
   float nameWidth = metrics.width( m_name.c_str() );
   float fontHeight = metrics.height();

   float textMarginSize = 3.0f;

   switch( m_blockSide )
   {
   case GBSP_INPUT:
      {
         m_bounds = QRectF( QPointF( -10.0f, -5.0f ), QSizeF( 10.0f, 10.0f ) );
         m_nameBounds = QRectF( QPointF( textMarginSize, -fontHeight * 0.5f ), QSizeF( nameWidth, fontHeight ) );
         break;
      }

   case GBSP_OUTPUT:
      {
         m_bounds = QRectF( QPointF( 0, -5.0f ), QSizeF( 10.0f, 10.0f ) );
         m_nameBounds = QRectF( QPointF( -nameWidth - textMarginSize, -fontHeight * 0.5f ), QSizeF( nameWidth, fontHeight ) );
         break;
      }
   }

   m_totalBounds = m_bounds.united( m_nameBounds );

   // recalculate bounds of the parent
   if ( m_parent )
   {
      m_parent->calculateBounds();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
   GraphLayout* graphScene = dynamic_cast< GraphLayout* >( scene() );

   if ( graphScene )
   {
      graphScene->startNegotiatingConnection( *this );
   }

   __super::mousePressEvent( event );
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
   __super::mouseReleaseEvent( event );

   GraphLayout* graphScene = dynamic_cast< GraphLayout* >( scene() );
   if ( graphScene )
   {
      GraphBlockSocket* destinationSocket = dynamic_cast< GraphBlockSocket* >( graphScene->itemAt( event->scenePos() ) );
      graphScene->finishNegotiatingConnection( destinationSocket );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool GraphBlockSocket::addConnection( GraphBlockConnection& connection )
{
   if ( onConnectionAdded( connection ) )
   {
      m_connections.push_back( &connection );
      return true;
   }
   else
   {
      return false;
   }
}

///////////////////////////////////////////////////////////////////////////////

bool GraphBlockSocket::isConnectedTo( GraphBlockSocket& socket ) const
{
   for( std::vector< GraphBlockConnection* >::const_iterator it = m_connections.begin(); it != m_connections.end(); ++it )
   {
      GraphBlockConnection* connection = *it;
      if ( ( &connection->getSource() == this ) && ( &connection->getDestination() == &socket ) )
      {
         return true;
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::removeConnection( GraphBlockConnection& connection )
{
   for( std::vector< GraphBlockConnection* >::iterator it = m_connections.begin(); it != m_connections.end(); ++it )
   {
      if ( *it == &connection )
      {
         onConnectionRemoved( connection );
         m_connections.erase( it );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GraphBlockConnection, Object );
   PROPERTY( GraphBlockSocket*, m_source );
   PROPERTY( GraphBlockSocket*, m_destination );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GraphBlockConnection* GraphBlockConnection::createConnection( GraphBlockSocket* source, GraphBlockSocket* destination )
{
   if ( !source || !destination )
   {
      return NULL;
   }

   GraphBlockConnection* connection = new GraphBlockConnection( source, destination );
    
   if ( source->addConnection( *connection ) == false )
   {
      return NULL;
   }

   if ( destination->addConnection( *connection ) == false )
   {
      source->removeConnection( *connection );
      return NULL;
   }
   
   return connection;
}

///////////////////////////////////////////////////////////////////////////////

GraphBlockConnection::GraphBlockConnection( GraphBlockSocket* source, GraphBlockSocket* destination )
   : m_source( source )
   , m_destination( destination )
{
   if ( m_source && m_destination )
   {
      calculateBounds();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockConnection::onObjectLoaded()
{
   __super::onObjectLoaded();

   ASSERT_MSG( m_source != NULL, "Information about this connection's source socket was not deserialized properly" );
   ASSERT_MSG( m_destination != NULL, "Information about this connection's destination socket was not deserialized properly" );

   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockConnection::calculateBounds()
{
   QPointF start = m_source->scenePos();
   QPointF end = m_destination->scenePos();

   setLine( start.rx(), start.ry(), end.rx(), end.ry() );
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockConnection::onRemoved()
{
   m_source->removeConnection( *this );
   m_destination->removeConnection( *this );
   m_source = NULL;
   m_destination = NULL;
}

///////////////////////////////////////////////////////////////////////////////
