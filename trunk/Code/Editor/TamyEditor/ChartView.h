/// @file   TamyEditor/ChartView.h
/// @brief  a view that displays a chart scene
#pragma once

#include <QGraphicsView>
#include "core-AppFlow/TimeDependent.h"


///////////////////////////////////////////////////////////////////////////////

class ChartScene;

///////////////////////////////////////////////////////////////////////////////

/**
 * A view that displays a chart scene.
 */
class ChartView : public QGraphicsView, public TimeDependent
{
   Q_OBJECT

private:
   ChartScene*             m_scene;
   float                   m_scale;

   QFont                   m_font;
   QBrush                  m_bgBrush;
   QPen                    m_axisPen;
   QPen                    m_gridPen;
   QPen                    m_textPen;

public:
   /**
    * Constructor.
    *
    * @param scene         displayed scene
    * @param parentWidget
    */
   ChartView( ChartScene* scene, QWidget* parentWidget );

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );

protected:
   // -------------------------------------------------------------------------
   // QGraphicsView implementation
   // -------------------------------------------------------------------------
   void wheelEvent( QWheelEvent* wheelEvent );
   void drawBackground( QPainter* painter, const QRectF& rect );
};

///////////////////////////////////////////////////////////////////////////////
