/// @file   TamyEditor/ChartScene.h
/// @brief  chart scene
#pragma once

#include <QGraphicsScene>
#include <QBrush>
#include <QPen>


///////////////////////////////////////////////////////////////////////////////

/**
 * Chart scene.
 */
class ChartScene : public QGraphicsScene
{
private:
   float             m_scale;

   QBrush                  m_bgBrush;
   QPen                    m_axisPen;
   QPen                    m_gridPen;
   QPen                    m_textPen;

public:
   /**
    * Constructor.
    */
   ChartScene();
   virtual ~ChartScene();

   /**
    * Returns current scale of the chart.
    */
   inline float getScale() const { return m_scale; }

protected:
   // -------------------------------------------------------------------------
   // QGraphicsScene
   // -------------------------------------------------------------------------
   void drawBackground( QPainter* painter, const QRectF& rect );
   void wheelEvent( QGraphicsSceneWheelEvent* wheelEvent );

private:
   int getAxisValues( const QRectF& rect, bool vertical, std::vector< float >& outPositions, std::vector< float >& outValues ) const;
};

///////////////////////////////////////////////////////////////////////////////
