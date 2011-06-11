#include "ChartLine.h"
#include <QPainter>
#include "ChartScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

ChartLine::ChartLine( ChartScene& scene, const QColor& color, float width, ChartLinePointsProvider* pointsProvider )
   : QGraphicsItem( NULL )
   , m_scene( scene )
   , m_color( color )
   , m_pen( QBrush( color ), width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
   , m_pointsProvider( pointsProvider )
   , m_points( NULL )
   , m_scaledPoints( NULL )
   , m_pointsCount( 0 )
{
   getLineData();
   updateLine();
}

///////////////////////////////////////////////////////////////////////////////

ChartLine::~ChartLine()
{
   delete m_pointsProvider;
   m_pointsProvider = NULL;

   delete [] m_points;
   m_points = NULL;

   delete [] m_scaledPoints;
   m_scaledPoints = NULL;
   
   m_pointsCount = 0;
}

///////////////////////////////////////////////////////////////////////////////

void ChartLine::getLineData()
{
   // reset previous values
   delete [] m_points;
   m_points = NULL;

   delete [] m_scaledPoints;
   m_scaledPoints = NULL;

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

   if ( m_pointsCount > 0 )
   {
      m_scaledPoints = new QPointF[ m_pointsCount ];
   }

   // add the keys onto the point markers
   for ( unsigned int i = 0; i < m_pointsCount; ++i )
   {
      ChartLineKeyValue* keyVal = new ChartLineKeyValue( *this, m_color, i );
      m_keys.push_back( keyVal );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ChartLine::updateLine()
{
   float scale = m_scene.getScale();

   // calculate the scaled points
   for ( unsigned int i = 0; i < m_pointsCount; ++i )
   {
      m_scaledPoints[i] = m_points[i] / scale;
   }

   // calculate the bounding rect
   float left, right, top, bottom;
   left = top = FLT_MAX;
   right = bottom = -FLT_MAX;
   for ( unsigned int i = 0; i < m_pointsCount; ++i )
   {
      if ( m_scaledPoints[i].x() < left ) { left = m_scaledPoints[i].x(); }
      if ( m_scaledPoints[i].x() > right ) { right = m_scaledPoints[i].x(); }
      if ( m_scaledPoints[i].y() < top ) { top = m_scaledPoints[i].y(); }
      if ( m_scaledPoints[i].y() > bottom ) { bottom = m_scaledPoints[i].y(); }
   }

   if ( m_pointsCount > 0 )
   {
      m_rect = QRectF( left, top, right - left, bottom - top );
   }

   // update the positions of the respective keys
   for ( unsigned int i = 0; i < m_pointsCount; ++i )
   {
      m_keys[i]->setPos( m_scaledPoints[i] );
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

   // update the bounding rectangle
   float scale = m_scene.getScale();
   m_scaledPoints[keyIdx] = m_points[keyIdx] / scale;

   // calculate the bounding rect
   if ( m_scaledPoints[keyIdx].x() < m_rect.left() ) { m_rect.setLeft( m_scaledPoints[keyIdx].x() ); }
   if ( m_scaledPoints[keyIdx].x() > m_rect.right() ) { m_rect.setRight( m_scaledPoints[keyIdx].x() ); }
   if ( m_scaledPoints[keyIdx].y() < m_rect.top() ) { m_rect.setTop( m_scaledPoints[keyIdx].y() ); }
   if ( m_scaledPoints[keyIdx].y() > m_rect.bottom() ) { m_rect.setBottom( m_scaledPoints[keyIdx].y() ); }

   // update the positions of the key widget
   m_keys[keyIdx]->setPos( m_scaledPoints[keyIdx] );
}

///////////////////////////////////////////////////////////////////////////////

void ChartLine::onScaleChanged()
{
   updateLine();
}

///////////////////////////////////////////////////////////////////////////////

void ChartLine::onKeyValueChanged( unsigned int keyIdx )
{
   ASSERT_MSG( keyIdx < m_keys.size(), "Invalid chart line key index" );
   if ( keyIdx >= m_keys.size() )
   {
      return;
   }

   float scale = m_scene.getScale();
   QPointF pos = m_keys[keyIdx]->pos();
   pos *= scale;

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
   if ( m_scaledPoints == NULL || m_pointsCount == 0 )
   {
      return;
   }

   painter->save();

   painter->setPen( m_pen );
   painter->setRenderHint( QPainter::Antialiasing, true );
   painter->drawPolyline( m_scaledPoints, m_pointsCount );

   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ChartLineKeyValue::ChartLineKeyValue( ChartLine& parent, const QColor& color, unsigned int pointIdx )
   : QGraphicsItem( &parent )
   , m_parentLine( parent )
   , m_pen( QBrush( color.darker( 150 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
   , m_unfocusedBrush( color.lighter( 120 ) )
   , m_focusedBrush( color.lighter( 150 ) )
   , m_pointIdx( pointIdx )
   , m_focused( false )
{
   m_rect.setLeft( -3.0f );
   m_rect.setTop( -3.0f );
   m_rect.setWidth( 6.0f );
   m_rect.setHeight( 6.0f );

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
   painter->save();

   painter->setPen( m_pen );
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
