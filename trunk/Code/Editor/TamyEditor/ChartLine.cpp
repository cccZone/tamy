#include "ChartLine.h"
#include <QPainter>
#include "ChartScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

ChartLine::ChartLine( ChartScene& scene, const QColor& color, float width, ChartLinePointsProvider* pointsProvider )
   : m_scene( scene )
   , m_color( color )
   , m_penWidth( width )
   , m_pen( QBrush( color ), width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
   , m_pointsProvider( pointsProvider )
   , m_points( NULL )
   , m_pointsCount( 0 )
{
   getLineData();
}

///////////////////////////////////////////////////////////////////////////////

ChartLine::~ChartLine()
{
   delete m_pointsProvider;
   m_pointsProvider = NULL;

   delete [] m_points;
   m_points = NULL;
   
   m_pointsCount = 0;
}

///////////////////////////////////////////////////////////////////////////////

void ChartLine::getLineData()
{
   // reset previous values
   delete [] m_points;
   m_points = NULL;

   m_pointsCount = 0;
   m_rect = QRectF();

   QList< QGraphicsItem* > children = childItems();
   foreach( QGraphicsItem* item, children )
   {
      scene()->removeItem( item );
      delete item;
   }
   m_keys.clear();

   // get the new data
   if ( m_pointsProvider != NULL )
   {
      m_pointsProvider->getPoints( m_points, m_pointsCount );
   }
   // add the keys onto the point markers
   for ( unsigned int i = 0; i < m_pointsCount; ++i )
   {
      ChartLineKeyValue* keyVal = new ChartLineKeyValue( *this, m_color, i );
      keyVal->setPos( m_points[i] );
      m_keys.push_back( keyVal );

      // calculate the bounding rect
      if ( m_points[i].x() < m_rect.left() ) { m_rect.setLeft( m_points[i].x() ); }
      if ( m_points[i].x() > m_rect.right() ) { m_rect.setRight( m_points[i].x() ); }
      if ( m_points[i].y() < m_rect.top() ) { m_rect.setTop( m_points[i].y() ); }
      if ( m_points[i].y() > m_rect.bottom() ) { m_rect.setBottom( m_points[i].y() ); }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ChartLine::updateKey( unsigned int keyIdx )
{
   if ( m_pointsProvider == NULL )
   {
      return;
   }

   // get the new position of the key
   m_points[keyIdx] = m_pointsProvider->getPoint( keyIdx );

   // calculate the bounding rect
   if ( m_points[keyIdx].x() < m_rect.left() ) { m_rect.setLeft( m_points[keyIdx].x() ); }
   if ( m_points[keyIdx].x() > m_rect.right() ) { m_rect.setRight( m_points[keyIdx].x() ); }
   if ( m_points[keyIdx].y() < m_rect.top() ) { m_rect.setTop( m_points[keyIdx].y() ); }
   if ( m_points[keyIdx].y() > m_rect.bottom() ) { m_rect.setBottom( m_points[keyIdx].y() ); }

   // update the positions of the key widget
   m_keys[keyIdx]->setPos( m_points[keyIdx] );
}

///////////////////////////////////////////////////////////////////////////////

void ChartLine::onKeyValueChanged( unsigned int keyIdx )
{
   ASSERT_MSG( keyIdx < m_keys.size(), "Invalid chart line key index" );
   if ( keyIdx >= m_keys.size() )
   {
      return;
   }

   QPointF pos = m_keys[keyIdx]->pos();
   m_pointsProvider->setPointPos( keyIdx, pos );
   
   // update the key position
   updateKey( keyIdx );

   // recalculate scene bounding rectangle
   QRectF rect = m_scene.itemsBoundingRect();
   m_scene.setSceneRect( rect );

   // redraw the scene
   m_scene.update();
}

///////////////////////////////////////////////////////////////////////////////

QRectF ChartLine::boundingRect() const 
{ 
   return m_rect; 
}

///////////////////////////////////////////////////////////////////////////////

void ChartLine::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
   if ( m_points == NULL || m_pointsCount == 0 )
   {
      return;
   }

   const QTransform& deviceTransform = painter->deviceTransform();
   float horizScale = deviceTransform.m11();
   float currPenWidth = m_penWidth / horizScale;
   m_pen.setWidthF( currPenWidth );

   painter->save();

   painter->setPen( m_pen );
   painter->setRenderHint( QPainter::Antialiasing, true );
   painter->drawPolyline( m_points, m_pointsCount );

   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ChartLineKeyValue::ChartLineKeyValue( ChartLine& parent, const QColor& color, unsigned int pointIdx )
   : QGraphicsItem( &parent )
   , m_parentLine( parent )
   , m_regularPen( QBrush( color.darker( 150 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
   , m_selectedPen( QBrush( QColor( 255, 226, 96 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
   , m_unfocusedBrush( color.lighter( 120 ) )
   , m_focusedBrush( color.lighter( 150 ) )
   , m_pointIdx( pointIdx )
   , m_focused( false )
{
   m_rect.setLeft( -3 );
   m_rect.setTop( -3 );
   m_rect.setWidth( 6 );
   m_rect.setHeight( 6 );

   setFlag( QGraphicsItem::ItemIsSelectable, true );
   setFlag( QGraphicsItem::ItemIsMovable, true );
   setAcceptTouchEvents( true );
   setAcceptedMouseButtons( Qt::LeftButton );
   setAcceptHoverEvents( true );
}

///////////////////////////////////////////////////////////////////////////////

ChartLineKeyValue::~ChartLineKeyValue()
{

}

///////////////////////////////////////////////////////////////////////////////

void ChartLineKeyValue::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
   const QTransform& deviceTransform = painter->deviceTransform();
   float horizScale = deviceTransform.m11();
   m_selectedPen.setWidthF( 1.f / horizScale );
   m_regularPen.setWidthF( 1.f / horizScale );
   m_rect.setCoords( -3.0f / horizScale, -3.0f / horizScale, 3.0f / horizScale, 3.0f / horizScale );

   painter->save();

   painter->setPen( isSelected() ? m_selectedPen : m_regularPen );
   painter->setBrush( m_focused ? m_focusedBrush : m_unfocusedBrush );
   painter->drawRect( m_rect );

   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////

void ChartLineKeyValue::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
   QPointF prevPos = pos();
   QPointF newMousePos = event->pos();
   QPointF newPos = prevPos + newMousePos;
   setPos( newPos );
   event->accept();

   m_parentLine.onKeyValueChanged( m_pointIdx );
}

///////////////////////////////////////////////////////////////////////////////

void ChartLineKeyValue::hoverEnterEvent( QGraphicsSceneHoverEvent* event )
{
   __super::hoverEnterEvent( event );
   m_focused = true;
}

///////////////////////////////////////////////////////////////////////////////

void ChartLineKeyValue::hoverLeaveEvent( QGraphicsSceneHoverEvent* event )
{
   __super::hoverLeaveEvent( event );
   m_focused = false;
}

///////////////////////////////////////////////////////////////////////////////
