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
QPen GraphBlock::s_borderPen( QBrush( QColor( 0, 0, 0 ) ), 3.0f );
QPen GraphBlock::s_selectionPen( QBrush( QColor( 255, 226, 96 ) ), 3.0f );

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

   painter->setRenderHint( QPainter::Antialiasing, true );
   painter->setRenderHint( QPainter::TextAntialiasing, true );
   painter->setFont( m_font );
   // draw the header
   {
      painter->setPen( isSelected() ? s_selectionPen : s_borderPen );
      painter->setBrush( fillColor.darker( 200 ) );

      switch( shape )
      {
      case GBS_RECTANGLE:
         {
            painter->drawRect( m_captionBounds );
            break;
         }

      case GBS_CIRCLE:
         {
            painter->drawArc( m_captionBounds, 0.0f, 16.0f * 180.0f );
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
      painter->setPen( isSelected() ? s_selectionPen : s_borderPen );
      painter->setBrush( fillColor );
      painter->drawRect( m_bounds );
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

GraphBlockSocket& GraphBlock::addSocket()
{
   GraphBlockSocket* socket = new GraphBlockSocket( this );
   m_sockets.push_back( socket );

   calculateBounds();

   return *socket;
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
      case GBSP_LEFT:
         {
            longestLeftSocketName = max( longestLeftSocketName, socket->getNameWidth() );
            ++leftSocketsCount;
            break;
         }
      
      case GBSP_RIGHT:
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
      case GBSP_LEFT:
         {
            socket->setPos( blockWidth, captionHeight + leftSocketY );
            leftSocketY += leftSocketsSpacing;
            break;
         }

      case GBSP_RIGHT:
         {
            socket->setPos( 0, captionHeight + rightSocketY );
            rightSocketY += rightSocketsSpacing;
            break;
         }
      }

      m_totalBounds = m_totalBounds.united( socket->boundingRect() );
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

GraphBlockSocket::GraphBlockSocket( GraphBlock* parent )
   : QGraphicsItem( parent )
   , m_parent( parent )
   , m_blockSide( GBSP_LEFT )
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
   if ( m_parent )
   {
      m_parent->calculateBounds();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::initialize( GraphBlockSocketPosition pos, const char* name )
{ 
   m_blockSide = pos;
   m_name = name;

   calculateBounds();
   if ( m_parent )
   {
      m_parent->calculateBounds();
   }
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
   painter->setRenderHint( QPainter::Antialiasing, true );
   painter->setRenderHint( QPainter::TextAntialiasing, true );

   painter->setPen( QPen( QBrush( QColor( 0, 0, 0 ) ), 1.0f ) );
   painter->setBrush( QColor( 100, 100, 100 ) );
   painter->drawRect( m_bounds );

   painter->setFont( m_font );
   painter->setBrush( QColor( 0, 0, 0 ) );
   painter->drawText( m_nameBounds, m_name.c_str(), QTextOption( Qt::AlignCenter ) );
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
   case GBSP_LEFT:
      {
         m_bounds = QRectF( QPointF( 0, -5.0f ), QSizeF( 10.0f, 10.0f ) );
         m_nameBounds = QRectF( QPointF( -nameWidth - textMarginSize, -fontHeight * 0.5f ), QSizeF( nameWidth, fontHeight ) );
         break;
      }

   case GBSP_RIGHT:
      {
         m_bounds = QRectF( QPointF( -10.0f, -5.0f ), QSizeF( 10.0f, 10.0f ) );
         m_nameBounds = QRectF( QPointF( textMarginSize, -fontHeight * 0.5f ), QSizeF( nameWidth, fontHeight ) );
         break;
      }
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

   event->accept();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
   GraphLayout* graphScene = dynamic_cast< GraphLayout* >( scene() );
   if ( graphScene )
   {
      GraphBlockSocket* destinationSocket = dynamic_cast< GraphBlockSocket* >( graphScene->itemAt( event->scenePos() ) );
      graphScene->finishNegotiatingConnection( *destinationSocket );
   }

   event->accept();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockSocket::addConnection( GraphBlockConnection* connection )
{
   ASSERT_MSG( connection != NULL, "NULL pointer instead a GraphBlockConnection instance" );
   if ( connection == NULL )
   {
      return;
   }
   m_connections.push_back( connection );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GraphBlockConnection, Object );
   PROPERTY( GraphBlockSocket*, m_source );
   PROPERTY( GraphBlockSocket*, m_destination );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GraphBlockConnection::GraphBlockConnection( GraphBlockSocket* source, GraphBlockSocket* destination )
   : QGraphicsItem( source )
   , m_source( source )
   , m_destination( destination )
{
   source->addConnection( this );
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockConnection::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
   painter->setRenderHint( QPainter::Antialiasing, true );

   QPointF start, end;
   start = m_source->pos();
   end = m_source->mapFromItem( m_destination, m_destination->pos() );
   painter->drawLine( start, end );

   // TODO: uwzglednic polaczenia w bound'sach danego bloczka
   // TODO: rozkminic jakk przemapowac pozycje, zeby sie dobrze rysowalo ( chyba trzeba do zrodlowego polaczenia mapowac )
   // TODO: zapamietywanie node'a we wlasciwym resource'ie i w bloczku go opisujacym - zeby to byly wskazniki na ta sama instancje
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockConnection::onObjectLoaded()
{
   __super::onObjectLoaded();

   ASSERT_MSG( m_source != NULL, "Information about this connection's source socket was not deserialized properly" );
   ASSERT_MSG( m_destination != NULL, "Information about this connection's destination socket was not deserialized properly" );
   setParentItem( m_source );

   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlockConnection::calculateBounds()
{
   m_bounds = m_source->boundingRect();
   m_bounds = m_bounds.united( m_destination->boundingRect() );
}

///////////////////////////////////////////////////////////////////////////////
