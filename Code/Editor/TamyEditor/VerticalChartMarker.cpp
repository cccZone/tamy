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

   // recalculate the bounding rectangle
   float value = m_valuePovider->getValue();
   QRectF modelRect( value, -FLT_MAX, value, FLT_MAX );
   chart->modelToScene( modelRect, m_sceneRect );
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
   m_pen.setWidthF( 1.f / horizScale );

   // draw the item
   painter->save();

   painter->setPen( m_pen );
   painter->drawLine( m_sceneRect.left(), -100000, m_sceneRect.left(), 100000 );

   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////
