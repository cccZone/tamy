/// @file   TamyEditor\ColorFrame.h
/// @brief  Widget showing a color
#pragma once

#include <QFrame.h>


///////////////////////////////////////////////////////////////////////////////

class Color;

///////////////////////////////////////////////////////////////////////////////

/**
 * Widget showing a color.
 */
class ColorFrame : public QFrame
{
   Q_OBJECT

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param color      engine color
    */
   ColorFrame( QWidget* parent, const Color& color );

   /**
    * Constructor.
    *
    * @param parent
    * @param color      qt color
    */
   ColorFrame( QWidget* parent, const QColor& color );

   /**
    * Returns the displayed color value.
    */
   QColor getColor() const;

   /**
    * Returns the displayed engine color value.
    */
   Color getEngineColor() const;

   /**
    * Sets the new color value.
    */
   void setColor( const Color& color );

signals:
   /**
    * Signal emitted when the color value changes.
    */
   void changed( const QColor& );

protected:
   void mouseDoubleClickEvent( QMouseEvent* event );
};

///////////////////////////////////////////////////////////////////////////////
