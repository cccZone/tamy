#include "ChartLine.h"
#include <QPainter>


///////////////////////////////////////////////////////////////////////////////

ChartLine::ChartLine( const QColor& color, float width, ChartLinePointsProvider* pointsProvider )
   :  QGraphicsItem( NULL )
   , m_pen( QBrush( color ), width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin )
   , m_pointsProvider( pointsProvider )
{

}

///////////////////////////////////////////////////////////////////////////////

ChartLine::~ChartLine()
{
   delete m_pointsProvider;
   m_pointsProvider = NULL;
}

///////////////////////////////////////////////////////////////////////////////

QRectF ChartLine::boundingRect() const 
{ 
   QPointF* points = NULL;
   unsigned int count = 0;

   if ( m_pointsProvider != NULL )
   {
      m_pointsProvider->getPoints( points, count );
   }

   QRectF rect;
   for ( unsigned int i = 0; i < count; ++i )
   {
      rect = rect.unite( QRectF( points[i], points[i] ) );
   }

   delete [] points;
   return rect;
}

///////////////////////////////////////////////////////////////////////////////

void ChartLine::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
   if ( m_pointsProvider == NULL )
   {
      return;
   }

   painter->setPen( m_pen );
   painter->setRenderHint( QPainter::Antialiasing, true );

   QPointF* points = NULL;
   unsigned int count = 0;
   m_pointsProvider->getPoints( points, count );
   painter->drawPolyline( points, count );
   delete [] points;
}

///////////////////////////////////////////////////////////////////////////////
