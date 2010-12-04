/// @file      TamyEditor/GraphBlock.h
/// @brief     a graphical representation of a graph node
#pragma once

#include <QGraphicsItem>
#include <QColor>
#include <QRectF>
#include <QPen>
#include <QFont>
#include "core/Object.h"


///////////////////////////////////////////////////////////////////////////////

class QPainter;
class QGraphicsScene;

///////////////////////////////////////////////////////////////////////////////

/**
 * A graphical representation of a graph node.
 */
class GraphBlock :  public Object, public QGraphicsItem
{
   DECLARE_CLASS( GraphBlock )

public:
   enum Shape
   {
      GBS_RECTANGLE,
      GBS_CIRCLE,
      GBS_ROUNDED,
   };

   enum SocketPosition
   {
      GBSP_LEFT,
      GBSP_RIGHT,
   };

private:
   std::string       m_caption;

   QPointF           m_position;
   QRectF            m_bounds;
   QRectF            m_captionBounds;

   static QPen       s_textPen;
   static QPen       s_borderPen;
   static QPen       s_selectionPen;
   QFont             m_font;

public:
   /**
    * Constructor.
    */
   GraphBlock();
   virtual ~GraphBlock();

   /**
    * Sets the caption of the block.h
    *
    * @param caption
    */
   void setCaption( const char* caption );

   /**
    * Checks if the block overlaps the specified position.
    * 
    * @param pos
    */
   bool doesOverlap( const QPointF& pos ) const;

   /**
    * Returns the represented node instance.
    */
   virtual Object* getNode() const { return NULL; }

   /**
    * Caches the block's state for storing purposes.
    */
   void saveState();

   /**
    * Restores the cached block state after the load.
    */
   void restoreState();

   // -------------------------------------------------------------------------
   // QGraphicsItem implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the bounds of the block.
    */
   inline QRectF boundingRect() const { return m_bounds; }

   /**
    * Paints the block.
    */
   void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onPropertyChanged( Property& property );

protected:
   // -------------------------------------------------------------------------
   // Block layout settings
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }

   /**
    * Returns the color of the block background.
    */
   virtual QColor getBgColor() const { return QColor( 255, 255, 255 ); }

   /**
    * Adds a new socket to the block.
    *
    * @param position      socket position
    * @param name          socket name
    */
   void addSocket( SocketPosition position, const char* name );

private:
   void calculateBounds();
};

///////////////////////////////////////////////////////////////////////////////

