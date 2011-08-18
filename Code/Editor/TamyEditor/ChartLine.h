/// @file   TamyEditor/ChartLine.h
/// @brief  chart line related graphic items
#pragma once

#include <QGraphicsItem>
#include <QColor>
#include <QRectF>
#include <QPen>
#include <QFont>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class ChartLineKeyValue;
class ChartScene;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;

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

   /**
    * Returns the value of the specified point.
    *
    * @param pointIdx
    */
   virtual QPointF getPoint( unsigned int pointIdx ) const = 0;

   /**
    * Changes the position of the specified point.
    *
    * @param pointIdx         index of the changed point
    * @param pos              new point position
    */
   virtual void setPointPos( unsigned int pointIdx, const QPointF& pos ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class ChartLine : public QGraphicsItem
{
private:
   ChartScene&                            m_scene;
   QColor                                 m_color;
   QPen                                   m_pen;
   ChartLinePointsProvider*               m_pointsProvider;

   QRectF                                 m_rect;
   QPointF*                               m_points;
   QPointF*                               m_scaledPoints;
   unsigned int                           m_pointsCount;

   std::vector< ChartLineKeyValue* >      m_keys;

public:
   /**
    * Constructor.
    *
    * @param scene               host chart scene
    * @param color               color of the line
    * @param width               line width
    * @param pointsProvider      line points provider
    */
   ChartLine( ChartScene& scene, const QColor& color, float width, ChartLinePointsProvider* pointsProvider );
   ~ChartLine();

   /**
    * Updates the line based on the data.
    */
   void updateLine();

   /**
    * Called when the scale of the parent chart scene changes.
    *
    * @param newScale
    */
   void onScaleChanged();

   /**
    * Called when a key value changes.
    */
   void onKeyValueChanged( unsigned int keyIdx );

   // -------------------------------------------------------------------------
   // QGraphicsItem implementation
   // -------------------------------------------------------------------------
   QRectF boundingRect() const;
   void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

private:
   void getLineData();
   void updateKey( unsigned int keyIdx );
};

///////////////////////////////////////////////////////////////////////////////

class ChartLineKeyValue : public QGraphicsItem
{
private:
   unsigned int               m_pointIdx;
   ChartLine&                 m_parentLine;

   QPen                       m_regularPen;
   QPen                       m_selectedPen;
   QBrush                     m_focusedBrush;
   QBrush                     m_unfocusedBrush;
   QRectF                     m_rect;
   bool                       m_focused;

public:
   /**
    * Constructor.
    *
    * @param parent              parent chart line
    * @param color               color of the key
    * @param pointIdx
    */
   ChartLineKeyValue( ChartLine& parent, const QColor& color, unsigned int pointIdx );
   ~ChartLineKeyValue();

   // -------------------------------------------------------------------------
   // QGraphicsItem implementation
   // -------------------------------------------------------------------------
   QRectF boundingRect() const { return m_rect; }
   void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

protected:
   void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
   void hoverEnterEvent( QGraphicsSceneHoverEvent* event );
   void hoverLeaveEvent( QGraphicsSceneHoverEvent* event );
};

///////////////////////////////////////////////////////////////////////////////
