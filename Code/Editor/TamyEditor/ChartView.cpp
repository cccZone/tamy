#include "ChartView.h"
#include "ChartScene.h"
#include <QWheelEvent>
#include "core/Algorithms.h"
#include <QImage>


///////////////////////////////////////////////////////////////////////////////

ChartView::ChartView( ChartScene* scene, QWidget* parentWidget )
   : QGraphicsView( scene, parentWidget )
   , m_scene( scene )
   , m_scale( 1.0f )
   , m_font( "Arial" )
{
   m_bgBrush = QBrush( QColor( 200, 200, 200 ) );
   m_axisPen = QPen( QBrush( QColor( 100, 100, 100 ) ), 3.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
   m_gridPen = QPen( QBrush( QColor( 150, 150, 150 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
   m_textPen = QPen( QBrush( QColor( 0, 0, 0 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

   setAutoFillBackground(false);
   setAttribute(Qt::WA_OpaquePaintEvent, true);
   setAttribute(Qt::WA_NoSystemBackground, true);

   // scale the view
   this->scale( m_scale, m_scale );
}

///////////////////////////////////////////////////////////////////////////////

void ChartView::wheelEvent( QWheelEvent* wheelEvent )
{
   const float delta = wheelEvent->delta();

   // unscale before we apply a new scale so that we can later on
   // scale by the desired amount
   this->scale( 1.0f / m_scale, 1.0f / m_scale );

   if ( delta < 0.0f )
   {
      m_scale *= 1.1f;
   }
   else
   {
      m_scale /= 1.1f;
   }

   m_scale = clamp( m_scale, 0.02f, 7.0f );
   wheelEvent->accept();

   // scale the view
   this->scale( m_scale, m_scale );
   QRectF sRect = m_scene->sceneRect();
   setSceneRect( sRect );
}


///////////////////////////////////////////////////////////////////////////////

void ChartView::drawBackground( QPainter* painter, const QRectF& rect )
{
   painter->save();


   // scale the pen sizes
   float fontHeight = 10.0f / m_scale;
   m_font.setPointSizeF( fontHeight );
   m_axisPen.setWidthF( 3.0f / m_scale );
   m_gridPen.setWidthF( 1.0f / m_scale );
   m_textPen.setWidthF( 1.0f / m_scale );

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
   painter->setFont( m_font );

   float frameWidth = this->rect().width();
   float viewportWidth = rect.width();
   float scale = viewportWidth / frameWidth;
   float scaleMag = mag( scale ) * 300.0f;

   // calculate the number of zeros that should be displayed for the values
   char strValue[32];
   char format[16];
   int dispScalePow;
   if ( scale < 1.0f )
   {
      dispScalePow = std::max( 0, (int)log10( 1.0f / scaleMag ) ) + 1;
   }
   else
   {
      dispScalePow = 1;
   }
   sprintf_s( format, "%%.%d%s", std::max( 1, dispScalePow ), "f" );

   // vertical axes
   {
      float startVal = rect.left();
      float endVal = rect.right();

      float displayedStartValue = roundEx( startVal, scaleMag ) - scaleMag; 
      float displayedEndValue = roundEx( endVal, scaleMag ) + scaleMag;
      int linesCount = ( displayedEndValue - displayedStartValue ) / scaleMag;
      int maxVisibleCaptions = 10;
      int visibleTextSpacing = linesCount / maxVisibleCaptions;
      if ( visibleTextSpacing <= 0 )
      {
         visibleTextSpacing = 1;
      }
      int textIdx = 0;
      for ( float x = displayedStartValue; x <= displayedEndValue; x += scaleMag, ++textIdx )
      {
         painter->setPen( m_gridPen );
         painter->drawLine( QPointF( x, rect.top() ),  QPointF( x, rect.bottom() ) );

         if ( textIdx % visibleTextSpacing == 0 )
         {
            sprintf_s( strValue, format, x );
            painter->setPen( m_textPen );
            painter->drawText( QPointF( x, rect.top() + fontHeight ), strValue );
         }
      }
   }

   // horizontal axes
   {
      float startVal = rect.top();
      float endVal = rect.bottom();

      float displayedStartValue = roundEx( startVal, scaleMag ) - scaleMag; 
      float displayedEndValue = roundEx( endVal, scaleMag ) + scaleMag;
      for ( float y = displayedStartValue; y <= displayedEndValue; y += scaleMag )
      {
         painter->setPen( m_gridPen );
         painter->drawLine( QPointF( rect.left(), y ),  QPointF( rect.right(), y ) );

         sprintf_s( strValue, format, y );
         painter->setPen( m_textPen );
         painter->drawText( QPointF( rect.left(), y ), strValue );
      }
   }

   painter->restore();
}

///////////////////////////////////////////////////////////////////////////////

void ChartView::update( float timeElapsed )
{
   m_scene->update();
}

///////////////////////////////////////////////////////////////////////////////
