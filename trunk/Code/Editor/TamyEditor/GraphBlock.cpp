#include "GraphBlock.h"
#include "GraphLayout.h"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>
#include "core/Algorithms.h"
#include <algorithm>
#include "GraphWidgetUtils.h"
#include "GraphBlockSocket.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( GraphBlock );
   PARENT( ReflectionObject );
   PROPERTY_EDIT( "caption", std::string, m_caption );
   PROPERTY( QPointF, m_position );
   PROPERTY( std::vector< GraphBlockSocket* >, m_sockets );
END_OBJECT();

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
   , m_socketsFactory( NULL )
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
   m_socketsFactory = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::setCentralWidget( QWidget* widget )
{
   m_embeddedWidget = widget;
   m_centralWidget->setWidget( m_embeddedWidget );
   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::setSocketsFactory( SocketsFactory* socketsFactory )
{
   m_socketsFactory = socketsFactory;
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
      painter->setPen( GraphWidgetUtils::s_borderPen );

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

      GraphWidgetUtils::drawShadowedText( painter, m_captionBounds, m_caption.c_str(), QTextOption( Qt::AlignCenter ) );
   }


   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
   __super::mouseMoveEvent( event );

   static_cast< GraphLayout* >( scene() )->onBlockMoved();
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
   unsigned int count = m_sockets.size();
   for ( unsigned int socketIdx = 0; socketIdx < count; ++socketIdx )
   {
      GraphBlockSocket* socket = m_sockets[socketIdx];
      if ( socket->getPosition() == position && socket->getName() == socketName )
      {
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
            longestLeftSocketName = max2( longestLeftSocketName, socket->getNameWidth() );
            ++leftSocketsCount;
            break;
         }
      
      case GBSP_OUTPUT:
         {
            longestRightSocketName = max2( longestRightSocketName, socket->getNameWidth() );
            ++rightSocketsCount;
            break;
         }
      }
   }
   float socketSize = metrics.height() + 6;
   float socketsHeight = max2( leftSocketsCount, rightSocketsCount ) * socketSize;

   // calculate the bounds
   QRectF centralWidgetBounds = m_centralWidget->subWidgetRect( m_embeddedWidget );

   float blockWidth = max2( captionWidth, longestLeftSocketName + longestRightSocketName );
   blockWidth = max2( blockWidth, (float)centralWidgetBounds.width() ); // make sure the block isn't too narrow
   blockWidth = max2( blockWidth, 100.0f ); // make sure the block isn't too narrow
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
            socket->setPos( blockWidth - GraphWidgetUtils::g_socketWidth, socketsVertOffset + rightSocketY );
            rightSocketY += rightSocketsSpacing;
            break;
         }
      }
   }

   // include the size of the sockets in the block's bounding box
   m_totalBounds = QRectF( m_totalBounds.left() - 11, m_totalBounds.top(), m_totalBounds.right() + 21, m_totalBounds.bottom() ); 
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
