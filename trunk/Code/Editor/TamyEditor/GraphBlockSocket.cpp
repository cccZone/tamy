#include "GraphBlockSocket.h"
#include <QFontMetrics>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

#include "GraphLayout.h"
#include "GraphWidgetUtils.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GraphBlockSocket );
   PARENT( ReflectionObject );
   PROPERTY( GraphBlock*, m_parent );
   PROPERTY( QPointF, m_position );
   PROPERTY( int, m_blockSide );
   PROPERTY( std::string, m_name );
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

   painter->setPen( GraphWidgetUtils::s_borderPen );
   painter->setBrush( bgColor );
   painter->drawRect( m_bounds );

   painter->setFont( m_font );
   painter->setBrush( QColor( 0, 0, 0 ) );
   GraphWidgetUtils::drawShadowedText( painter, m_nameBounds, m_name.c_str(), QTextOption( Qt::AlignCenter ) );

   painter->restore();
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
         m_bounds = QRectF( QPointF( 0.0f, -GraphWidgetUtils::g_socketHeight * 0.5f), QSizeF( GraphWidgetUtils::g_socketWidth, GraphWidgetUtils::g_socketHeight ) );
         m_nameBounds = QRectF( QPointF( GraphWidgetUtils::g_socketWidth + textMarginSize, -fontHeight * 0.5f ), QSizeF( nameWidth, fontHeight ) );
         break;
      }

   case GBSP_OUTPUT:
      {
         m_bounds = QRectF( QPointF( 0, -GraphWidgetUtils::g_socketHeight * 0.5f ), QSizeF( GraphWidgetUtils::g_socketWidth, GraphWidgetUtils::g_socketHeight ) );
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
