/// @file   TamyEditor/ChartItem.h
/// @brief  a base chart item interface
#pragma once

#include <QGraphicsItem>


///////////////////////////////////////////////////////////////////////////////

/**
 * A base chart item interface.
 */
class ChartItem : public QGraphicsItem
{
public:
   virtual ~ChartItem() {}

protected:
   /**
    * Default constructor.
    */
   ChartItem() : QGraphicsItem( NULL ) {}
};

///////////////////////////////////////////////////////////////////////////////
