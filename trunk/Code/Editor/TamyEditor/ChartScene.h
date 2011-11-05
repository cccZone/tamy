/// @file   TamyEditor/ChartScene.h
/// @brief  a chart scene
#pragma once

#include <QGraphicsScene>


///////////////////////////////////////////////////////////////////////////////

/**
 * A chart scene.
 */
class ChartScene : public QGraphicsScene
{

public:
   /**
    * Constructor.
    */
   ChartScene();
   ~ChartScene();

   /**
    * Translates the model coordinates to the scene coordinates.
    */
   void modelToScene( const QRectF& modelRect, QRectF& sceneRect ) const;
};

///////////////////////////////////////////////////////////////////////////////
