#include "ChartScene.h"
#include <QPainter>
#include "ChartItem.h"


///////////////////////////////////////////////////////////////////////////////

ChartScene::ChartScene()
{
}

///////////////////////////////////////////////////////////////////////////////

ChartScene::~ChartScene()
{
}

///////////////////////////////////////////////////////////////////////////////

void ChartScene::modelToScene( const QRectF& mRect, QRectF& sRect ) const
{
   QRectF modelRect = sceneRect();

   sRect.setLeft( mRect.left() < modelRect.left() ? modelRect.left() : mRect.left() );
   sRect.setRight( mRect.right() > modelRect.right() ? modelRect.right() : mRect.right() );
   sRect.setTop( mRect.top() > modelRect.top() ? modelRect.top() : mRect.top() );
   sRect.setBottom( mRect.bottom() < modelRect.bottom() ? modelRect.bottom() : mRect.bottom() );
}

///////////////////////////////////////////////////////////////////////////////

