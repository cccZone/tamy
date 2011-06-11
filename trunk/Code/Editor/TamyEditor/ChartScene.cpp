#include "ChartScene.h"
#include <QPainter>
#include "core/Algorithms.h"
#include <QGraphicsSceneWheelEvent>
#include "ChartLine.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

ChartScene::ChartScene()
   : m_scale( 0.001f )
{
   m_bgBrush = QBrush( QColor( 200, 200, 200 ) );
   m_axisPen = QPen( QBrush( QColor( 100, 100, 100 ) ), 3.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
   m_gridPen = QPen( QBrush( QColor( 150, 150, 150 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
   m_textPen = QPen( QBrush( QColor( 0, 0, 0 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
}

///////////////////////////////////////////////////////////////////////////////

ChartScene::~ChartScene()
{

}

///////////////////////////////////////////////////////////////////////////////

void ChartScene::drawBackground( QPainter* painter, const QRectF& rect )
{
   painter->save();

   painter->setRenderHint( QPainter::Antialiasing, false );
   painter->setRenderHint( QPainter::TextAntialiasing, false );

   // draw a rectangle over the background
   painter->setBrush( m_bgBrush );
   painter->fillRect( rect, m_bgBrush );

   painter->setRenderHint( QPainter::Antialiasing, true );
   painter->setRenderHint( QPainter::TextAntialiasing, true );

   // draw the reference grid
   painter->setPen( m_axisPen );
   painter->drawLine( QPointF( rect.left(), 0 ),  QPointF( rect.right(), 0 ) );

   // vertical axes
   std::vector< float > positions;
   std::vector< float > values;
   int scalePow = getAxisValues( rect, true, positions, values );
   unsigned int count = positions.size();
   char strTime[32];
   char format[16];
   sprintf_s( format, "%%.%d%s", std::max( 1, scalePow ), "f" );
   for ( unsigned int i = 0; i < count; ++i )
   {
      painter->setPen( m_gridPen );
      painter->drawLine( QPointF( positions[i], rect.top() ),  QPointF( positions[i], rect.bottom() ) );

      painter->setPen( m_textPen );
      sprintf_s( strTime, format, values[i] );
      painter->drawText( QPointF( positions[i], rect.top() + 10 ), strTime );
   }

   // horizontal axes
   positions.clear();
   values.clear();
   scalePow = getAxisValues( rect, false, positions, values );
   count = positions.size();
   sprintf_s( format, "%%.%d%s", std::max( 1, scalePow ), "f" );
   for ( unsigned int i = 0; i < count; ++i )
   {
      painter->setPen( m_gridPen );
      painter->drawLine( QPointF( rect.left(), positions[i] ),  QPointF( rect.right(), positions[i] ) );

      painter->setPen( m_textPen );
      sprintf_s( strTime, format, values[i] );
      painter->drawText( QPointF( rect.left() + 10, positions[i] ), strTime );
   }

   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////

void ChartScene::wheelEvent( QGraphicsSceneWheelEvent* wheelEvent )
{
   const float delta = wheelEvent->delta();

   if ( delta < 0.0f )
   {
      m_scale *= 2.0f;
   }
   else
   {
      m_scale *= 0.5f;
   }
   wheelEvent->accept();

   // update the items
   QList< QGraphicsItem* > sceneItems = items();
   foreach( QGraphicsItem* item, sceneItems )
   {
      ChartLine* line = dynamic_cast< ChartLine* >( item );
      if ( line )
      {
         line->onScaleChanged();
      }
   }

   // recalculate scene bounding rectangle
   QRectF rect = itemsBoundingRect();
   setSceneRect( rect );

   // redraw the scene
   update();
}

///////////////////////////////////////////////////////////////////////////////

int ChartScene::getAxisValues( const QRectF& rect, bool vertical, std::vector< float >& outPositions, std::vector< float >& outValues ) const
{
   const float scale = getScale();

   float visibleStartValue, visibleEndValue;
   if ( vertical )
   {
      visibleStartValue = rect.left() * scale;
      visibleEndValue = rect.right() * scale;
   }
   else
   {
      visibleStartValue = rect.top() * scale;
      visibleEndValue = rect.bottom() * scale;
   }

   // calculate the values and their respective positions in the view
   float scaleMag = mag( scale ) * 100.0f;

   float displayedStartValue = roundEx( visibleStartValue, scaleMag ) - scaleMag; 
   float displayedEndValue = roundEx( visibleEndValue, scaleMag ) + scaleMag;

   const float unitSpacing = 100.0f;
   float spacing = unitSpacing * scale;
   for ( float i = displayedStartValue; i <= displayedEndValue; i += spacing )
   {
      outValues.push_back( i );
      outPositions.push_back( i / scale );
   }

   // calculate the number of zeros that should be displayed for the values
   int dispScalePow;
   if ( scale < 1.0f )
   {
      dispScalePow = std::max( 0, (int)log10( 1.0f / scaleMag ) ) + 1;
   }
   else
   {
      dispScalePow = 1;
   }
   return dispScalePow;
}

///////////////////////////////////////////////////////////////////////////////
