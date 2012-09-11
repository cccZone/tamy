#include "GraphBlock.h"
#include "GraphLayout.h"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>
#include "core/Algorithms.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( GraphBlock );
   PARENT( ReflectionObject );
   PROPERTY_EDIT( "caption", std::string, m_caption );
   PROPERTY( QPointF, m_position );
   PROPERTY( std::vector< GraphBlockSocket* >, m_sockets );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

QPen s_textPen( QBrush( QColor( 30, 30, 30 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
QPen s_textShadowPen( QBrush( QColor( 130, 130, 130 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
QPen s_borderPen( QBrush( QColor( 200, 200, 200 ) ), 0.5f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
QPen s_connectionPen( QBrush( QColor( 200, 200, 200 ) ), 2.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

float g_socketWidth = 10.0f;
float g_socketHeight = 10.0f;

///////////////////////////////////////////////////////////////////////////////

/**
 * A utility method for drawing a text with shadow behind it.
 */
static void drawShadowedText( QPainter* painter, const QRectF& rect, const QString& text, const QTextOption& options )
{
   painter->setPen( s_textShadowPen );
   QRectF textShadowRect = rect;
   textShadowRect.setLeft( textShadowRect.left() + 1.0f );
   textShadowRect.setRight( textShadowRect.right() + 1.0f );
   painter->drawText( textShadowRect, text, options );

   painter->setPen( s_textPen );
   painter->drawText( rect, text, options );
}

///////////////////////////////////////////////////////////////////////////////

#include <QLabel> 

GraphBlock::GraphBlock()
   : QGraphicsItem( NULL )
   , m_caption( "" )
   , m_bounds( QPointF( 0, 0 ), QSizeF( 100, 100 ) )
   , m_captionBounds( QPointF( 0, 0 ), QSizeF( 100, 0 ) )
   , m_font( "Verdana", 15, QFont::Light )
   , m_centralWidget( new QGraphicsProxyWidget( this ) )
   , m_embeddedWidget( NULL )
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

void GraphBlock::setCentralWidget( QWidget* widget )
{
   m_embeddedWidget = widget;
   m_centralWidget->setWidget( m_embeddedWidget );
   calculateBounds();
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
   QLinearGradient fillBrush( QPointF(0, 0), QPointF(0, m_captionBounds.height()));
   fillBrush.setColorAt( 0, isSelected() ? QColor( 255, 226, 96 ) : QColor( 200, 200, 200 ) );
   fillBrush.setColorAt( 0.5f, QColor( 70, 70, 70 ) );

   painter->save();

   painter->setRenderHint( QPainter::Antialiasing, true );
   painter->setRenderHint( QPainter::TextAntialiasing, true );
   painter->setFont( m_font );
   
   const float edgeRadius = 3.0f;

   // draw the block's layout
   {
      painter->setPen( s_borderPen );

      painter->setBrush( QColor( 150, 150, 150 ) );
      painter->setOpacity( 0.5f );
      painter->drawRoundedRect( m_bounds, edgeRadius, edgeRadius );
   }

   // draw the header
   {
      
      QPainterPath path;

      QRectF topBB = m_captionBounds;
      topBB.setHeight( m_captionBounds.height() + edgeRadius );

      QRectF bottomBB = m_captionBounds;
      bottomBB.setTop( m_captionBounds.height() );
      bottomBB.setHeight( edgeRadius );

      path.addRoundedRect( topBB, edgeRadius, edgeRadius );
      path.addRect( bottomBB );

      painter->setOpacity( 1.0f );
      painter->setPen( Qt::NoPen );
      painter->setBrush( fillBrush );
      painter->drawPath( path );

      drawShadowedText( painter, m_captionBounds, m_caption.c_str(), QTextOption( Qt::AlignCenter ) );
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
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::onPropertyChanged( ReflectionProperty& property )
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
   for ( std::set< std::string >::const_iterator removedSocketNameIt = socketNames.begin(); removedSocketNameIt != socketNames.end(); ++removedSocketNameIt )
   {
      removeSingleSocket( position, *removedSocketNameIt );
   }

   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::removeSocket( GraphBlockSocketPosition position, const std::string& socketName )
{
   removeSingleSocket( position, socketName );
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::removeSingleSocket( GraphBlockSocketPosition position, const std::string& socketName )
{
   GraphLayout* hostLayout = static_cast< GraphLayout* >( scene() );

   unsigned int count = m_sockets.size();
   for ( unsigned int socketIdx = 0; socketIdx < count; ++socketIdx )
   {
      GraphBlockSocket* socket = m_sockets[socketIdx];
      if ( socket->getPosition() == position && socket->getName() == socketName )
      {
         // remove the connections that bind this socket to the other ones
         if ( hostLayout )
         {
            std::vector< GraphBlockConnection* > connections = socket->getConnections();
            uint connectionsCount = connections.size();
            for( uint connIdx = 0; connIdx < connectionsCount; ++connIdx )
            {
               GraphBlockConnection* connection = connections[connIdx];
               hostLayout->removeConnection( *connection );
            }
         }

         // delete the socket
         delete socket;
         m_sockets.erase( m_sockets.begin() + socketIdx );
         break;
      }
   }
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
            longestLeftSocketName = std::max( longestLeftSocketName, socket->getNameWidth() );
            ++leftSocketsCount;
            break;
         }
      
      case GBSP_OUTPUT:
         {
            longestRightSocketName = std::max( longestRightSocketName, socket->getNameWidth() );
            ++rightSocketsCount;
            break;
         }
      }
   }
   float socketSize = metrics.height() + 6;
   float socketsHeight = std::max( leftSocketsCount, rightSocketsCount ) * socketSize;

   // calculate the bounds
   QRectF centralWidgetBounds = m_centralWidget->subWidgetRect( m_embeddedWidget );

   float blockWidth = std::max( captionWidth, longestLeftSocketName + longestRightSocketName );
   blockWidth = std::max( blockWidth, (float)centralWidgetBounds.width() ); // make sure the block isn't too narrow
   blockWidth = std::max( blockWidth, 100.0f ); // make sure the block isn't too narrow
   float blockHeight = captionHeight + centralWidgetBounds.height() + socketsHeight;

   // set the caption bounds
   m_captionBounds.setWidth( blockWidth );
   m_captionBounds.setHeight( captionHeight );
   m_totalBounds = m_captionBounds;

   // set the block bounds
   m_bounds.setTop( captionHeight );
   m_bounds.setWidth( blockWidth );
   m_bounds.setHeight( blockHeight );

   // calculate the bounds of this block
   m_totalBounds = m_totalBounds.united( m_bounds );

   // set the position of the central widget
   m_centralWidget->setPos( ( blockWidth - centralWidgetBounds.width() ) * 0.5f, captionHeight );

   // position the sockets
   float leftSocketsSpacing = ( leftSocketsCount > 0 ) ? ( socketsHeight / (float)leftSocketsCount ) : 0;
   float rightSocketsSpacing = ( rightSocketsCount > 0 ) ? ( socketsHeight / (float)rightSocketsCount ) : 0;

   float rightSocketY = rightSocketsSpacing * 0.5f;
   float leftSocketY = leftSocketsSpacing * 0.5f;
   float socketsVertOffset = captionHeight + centralWidgetBounds.height();
   for ( std::vector< GraphBlockSocket* >::const_iterator it = m_sockets.begin(); it != m_sockets.end(); ++it )
   {
      GraphBlockSocket* socket = *it;

      switch( socket->getPosition() )
      {
      case GBSP_INPUT:
         {
            socket->setPos( 0, socketsVertOffset + leftSocketY );
            leftSocketY += leftSocketsSpacing;
            break;
         }

      case GBSP_OUTPUT:
         {
            socket->setPos( blockWidth - g_socketWidth, socketsVertOffset + rightSocketY );
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

BEGIN_OBJECT( GraphBlockSocket );
   PARENT( ReflectionObject );
   PROPERTY( GraphBlock*, m_parent );
   PROPERTY( QPointF, m_position );
   PROPERTY( int, m_blockSide );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GraphBlockConnection* >, m_connections );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GraphBlockSocket::GraphBlockSocket()
   : QGraphicsItem( NULL )
   , m_parent( NULL )
   , m_blockSide( GBSP_INPUT )
   , m_name( "" )
   , m_font( "Verdana", 8, QFont::Light )
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

   if ( m_parent )
   {
      setParentItem( m_parent );
      setPos( m_position );
   }
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

   QColor bgColor = getBgColor();

   painter->setPen( s_borderPen );
   painter->setBrush( bgColor );
   painter->drawRect( m_bounds );

   painter->setFont( m_font );
   painter->setBrush( QColor( 0, 0, 0 ) );
   drawShadowedText( painter, m_nameBounds, m_name.c_str(), QTextOption( Qt::AlignCenter ) );

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
         m_bounds = QRectF( QPointF( 0.0f, -g_socketHeight * 0.5f), QSizeF( g_socketWidth, g_socketHeight ) );
         m_nameBounds = QRectF( QPointF( g_socketWidth + textMarginSize, -fontHeight * 0.5f ), QSizeF( nameWidth, fontHeight ) );
         break;
      }

   case GBSP_OUTPUT:
      {
         m_bounds = QRectF( QPointF( 0, -g_socketHeight * 0.5f ), QSizeF( g_socketWidth, g_socketHeight ) );
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

BEGIN_OBJECT( GraphBlockConnection );
   PARENT( ReflectionObject );
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
      delete connection;
      return NULL;
   }

   if ( destination->addConnection( *connection ) == false )
   {
      source->removeConnection( *connection );
      delete connection;
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

void GraphBlockConnection::calculateBounds()
{
   if ( !m_source || !m_destination )
   {
      return;
   }
   QPointF start = m_source->scenePos();
   QPointF end = m_destination->scenePos();

   setLine( start.rx(), start.ry(), end.rx(), end.ry() );
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockConnection::onRemoved()
{
   if ( m_source )
   {
      m_source->removeConnection( *this );
      m_source = NULL;
   }

   if ( m_destination )
   {
      m_destination->removeConnection( *this );
      m_destination = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

bool GraphBlockConnection::isOk() const
{
   if ( !m_source || !m_source->parentItem() )
   {
      return false;
   }

   if ( !m_destination || !m_destination->parentItem() )
   {
      return false;
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

QRectF GraphBlockConnection::boundingRect() const
{
   QLineF l = line();

   m_displayedPath = QPainterPath();
   m_displayedPath.moveTo( l.p1() );
   QPointF c1, c2;
   c1 = l.p1();
   c1.setX( c1.x() + 50.0f );

   c2 = l.p2();
   c2.setX( c2.x() - 50.0f );

   m_displayedPath.cubicTo( c1, c2, l.p2() );

   return m_displayedPath.boundingRect();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockConnection::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
   painter->save();

   painter->setRenderHint( QPainter::Antialiasing, true );
   painter->setBrush( Qt::NoBrush );
   painter->setPen( s_connectionPen );
   painter->drawPath( m_displayedPath );
   
   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////
