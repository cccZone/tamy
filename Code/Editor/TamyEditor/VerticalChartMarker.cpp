#include "VerticalChartMarker.h"
#include "ChartScene.h"
#include <QPainter>
#include <QWidget>


///////////////////////////////////////////////////////////////////////////////

VerticalChartMarker::VerticalChartMarker( VCMValueProvider* valuePovider )
   : m_valuePovider( valuePovider )
   , m_pen( QBrush( QColor( 30, 200, 100 ) ), 4.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
{
}

///////////////////////////////////////////////////////////////////////////////

VerticalChartMarker::~VerticalChartMarker()
{
   delete m_valuePovider;
   m_valuePovider = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void VerticalChartMarker::update( float timeElapsed )
{
   ChartScene* chart = dynamic_cast< ChartScene* >( scene() );
   if ( !chart )
   {
      return;
   }

   float prevPos = m_sceneRect.left();
   
   // recalculate the bounding rectangle
   float value = m_valuePovider->getValue();
   QRectF modelRect( value, -100000, 4.0f, 200000 );
   chart->modelToScene( modelRect, m_sceneRect );

   float XWIDTH = 50;
   if ( prevPos < value )
   {
      float width = value - prevPos + XWIDTH;
      chart->update( QRectF( prevPos - XWIDTH*0.5f, -100000, width, 200000 ) );
   }
   else
   {
      float width = prevPos - value + XWIDTH;
      chart->update( QRectF( value - XWIDTH*0.5f, -100000, width, 200000 ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

QRectF VerticalChartMarker::boundingRect() const
{
   return m_sceneRect;
}

///////////////////////////////////////////////////////////////////////////////

void VerticalChartMarker::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
   ChartScene* chart = dynamic_cast< ChartScene* >( scene() );
   if ( !chart )
   {
      return;
   }

   const QTransform& deviceTransform = painter->deviceTransform();
   float horizScale = deviceTransform.m11();
   m_pen.setWidthF( 4.f / horizScale );

   // draw the item
   painter->save();

   painter->setPen( m_pen );
   painter->drawLine( m_sceneRect.left(), m_sceneRect.top(), m_sceneRect.left(), m_sceneRect.height() );

   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////
