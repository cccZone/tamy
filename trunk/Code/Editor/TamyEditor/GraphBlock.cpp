#include "GraphBlock.h"
#include <QPainter>
#include <QPainterPath>
#include "core/Algorithms.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( GraphBlock, Object )
   PROPERTY_EDIT( "caption", std::string, m_caption )
   PROPERTY( QPointF, m_position )
   PROPERTY( QRectF, m_bounds )
END_OBJECT()

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

   // draw the block based on the shape
   switch( shape )
   {
   case GBS_RECTANGLE:
      {
         painter->setPen( isSelected() ? s_selectionPen : s_borderPen );
         {
            QPainterPath shapePath;
            shapePath.addRect( m_bounds );
            painter->setBrush( fillColor );
            painter->drawPath( shapePath );
   
            QPainterPath captionPath;
            captionPath.addRoundedRect( m_captionBounds, 1.0f, 1.0f );

            painter->setBrush( fillColor.darker() );
            painter->drawPath( captionPath );
         }

         QPainterPath textPath;
         textPath.addText( 3, m_captionBounds.height() - 3, m_font, m_caption.c_str() );
         painter->setPen( s_textPen );
         painter->setBrush( fillColor.darker( 400 ) );
         painter->drawPath( textPath );

         break;
      }

   case GBS_CIRCLE:
      {
         // TODO: !!!
         QPainterPath path;
         path.addEllipse( m_bounds );
         painter->setPen( isSelected() ? s_selectionPen : s_borderPen );
         painter->setBrush( fillColor );
         painter->drawPath( path );

         break;
      }

   case GBS_ROUNDED:
      {
         // TODO: !!!
         QPainterPath path;
         path.addRoundedRect( m_bounds, 3.0f, 3.0f );

         painter->setPen( isSelected() ? s_selectionPen : s_borderPen );
         painter->setBrush( fillColor );
         painter->drawPath( path );

         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::saveState()
{
   m_position = pos();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::restoreState()
{
   setPos( m_position );
   calculateBounds();
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::onPropertyChanged( Property& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_caption" )
   {
      /// recalculate the block bounds each time the caption changes
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

void GraphBlock::calculateBounds()
{
   QFontMetrics metrics( m_font );
   float width = metrics.width( m_caption.c_str() );
   width = clamp< float >( width, 100.0f, FLT_MAX );

   m_bounds.setWidth( width );

   m_captionBounds = m_bounds;
   m_captionBounds.setHeight( metrics.height() + 6 );
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::addSocket( SocketPosition position, const char* name )
{
   // TODO: !!!!!!!!!!! polaczenia
   m_sockets.push_back( new socket( position, name ) );
}

///////////////////////////////////////////////////////////////////////////////
