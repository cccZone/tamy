/// @file   TamyEditor/ChartLine.h
/// @brief  chart line related graphic items
#pragma once

#include <QGraphicsItem>
#include <QColor>
#include <QRectF>
#include <QPen>
#include <QFont>
#include <QGraphicsLineItem>


///////////////////////////////////////////////////////////////////////////////

class ChartLinePointsProvider
{
public:
   virtual ~ChartLinePointsProvider() {}

   /**
    * Allocates and fills an array of points the line is composed of.
    * CAUTION: The points are allocated using an array new allocator( new [] ).
    * The caller is responsible for releasing this array.
    *
    * @param outPoints     a reference to the points array
    * @param outCount      number of points in the array
    */
   virtual void getPoints( QPointF*& outPoints, unsigned int& outCount ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

class ChartLine : public QGraphicsItem
{
private:
   QPen                       m_pen;
   ChartLinePointsProvider*   m_pointsProvider;

public:
   /**
    * Constructor.
    *
    * @param color               color of the line
    * @param width               line width
    * @param pointsProvider      line points provider
    */
   ChartLine( const QColor& color, float width, ChartLinePointsProvider* pointsProvider );
   ~ChartLine();

   // -------------------------------------------------------------------------
   // QGraphicsItem implementation
   // -------------------------------------------------------------------------
   QRectF boundingRect() const;
   void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
};

///////////////////////////////////////////////////////////////////////////////
