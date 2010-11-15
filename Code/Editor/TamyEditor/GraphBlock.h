/// @file      TamyEditor/GraphBlock.h
/// @brief     a graphical representation of a graph node
#pragma once

#include <QGraphicsItem>
#include <QColor>
#include <QRectF>
#include <QPen>


///////////////////////////////////////////////////////////////////////////////

class QPainter;
class QGraphicsScene;

///////////////////////////////////////////////////////////////////////////////

/**
 * A graphical representation of a graph node.
 */
class GraphBlock : public QGraphicsItem
{
public:
   enum Shape
   {
      GBS_RECTANGLE,
      GBS_CIRCLE,
      GBS_ROUNDED,
   };

private:
   void*             m_node;

   Shape             m_shape;
   QRectF            m_bounds;

   QColor            m_fillColor;
   static QPen       s_borderPen;
   static QPen       s_selectionPen;

   QString           m_caption;

public:
   /**
    * Constructor.
    */
   GraphBlock( Shape shape, const QColor& fillColor, void* node );
   virtual ~GraphBlock();

   /**
    * Sets a new caption of the block.
    * 
    * @param caption
    */
   void setCaption( const QString& caption );

   /**
    * Checks if the block overlaps the specified position.
    * 
    * @param pos
    */
   bool doesOverlap( const QPointF& pos ) const;

   /**
    * Returns the represented node instance.
    */
   void* getNode() const { return m_node; }

   // -------------------------------------------------------------------------
   // QGraphicsItem implementation
   // -------------------------------------------------------------------------
   QRectF boundingRect() const { return m_bounds; }
   void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );
};

///////////////////////////////////////////////////////////////////////////////
