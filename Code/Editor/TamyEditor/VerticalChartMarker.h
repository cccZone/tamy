/// @file   TamyEditor/VerticalChartMarker.h
/// @brief  a widget that marks a specific value on the horizontal chart axis
#pragma once

#include "ChartItem.h"
#include <QPen>
#include "core-AppFlow/TimeDependent.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * An interface for accessing a value that should be marked on the chart.
 */
class VCMValueProvider
{
public:
   virtual ~VCMValueProvider() {}

   /**
    * Returns the value that should be marked on the chart.
    */
   virtual float getValue() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A widget that marks a specific value on the horizontal chart axis.
 */
class VerticalChartMarker : public ChartItem, public TimeDependent
{
private:
   VCMValueProvider*          m_valuePovider;
   QPen                       m_pen;

   // runtime values
   mutable QRectF             m_sceneRect;

public:
   /**
    * Constructor.
    *
    * @param valueProvider
    */
   VerticalChartMarker( VCMValueProvider* valuePovider );
   ~VerticalChartMarker();

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );

   // -------------------------------------------------------------------------
   // QGraphicsItem implementation
   // -------------------------------------------------------------------------
   QRectF boundingRect() const;
   void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
};

///////////////////////////////////////////////////////////////////////////////