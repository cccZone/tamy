#include "GraphBlock.h"
#include <QPainter>
#include <QPainterPath>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GraphBlock, Object )
   PROPERTY( Object*, m_node )
   PROPERTY( Shape, m_shape )
   PROPERTY( QPointF, m_position )
   PROPERTY( QRectF, m_bounds )
   PROPERTY( QColor, m_fillColor )
   PROPERTY( std::string, m_caption )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

QPen GraphBlock::s_borderPen( QBrush( QColor( 0, 0, 0 ) ), 3.0f );
QPen GraphBlock::s_selectionPen( QBrush( QColor( 255, 226, 96 ) ), 3.0f );

///////////////////////////////////////////////////////////////////////////////

GraphBlock::GraphBlock( Shape shape, const QColor& fillColor, Object* node )
   : QGraphicsItem( NULL )
   , m_node( node )
   , m_shape( shape )
   , m_bounds( QPointF( 0, 0 ), QSizeF( 100, 100 ) )
   , m_fillColor( fillColor )
{
   setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable );
}

///////////////////////////////////////////////////////////////////////////////

GraphBlock::~GraphBlock()
{
}

///////////////////////////////////////////////////////////////////////////////

void GraphBlock::setCaption( const std::string& caption )
{
   m_caption = caption;

   // TODO: recalculate the bounds to make sure the caption fits into the title bar

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
   QPainterPath path;

   // draw the block based on the shape
   switch( m_shape )
   {
   case GBS_RECTANGLE:
      {
         path.addRect( m_bounds );
         break;
      }

   case GBS_CIRCLE:
      {
         path.addEllipse( m_bounds );
         break;
      }

   case GBS_ROUNDED:
      {
         path.addRoundedRect( m_bounds, 0.5f, 0.5f );
         break;
      }
   }

   painter->setPen( isSelected() ? s_selectionPen : s_borderPen );
   painter->setBrush( m_fillColor );
   painter->drawPath( path );
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
}

///////////////////////////////////////////////////////////////////////////////
