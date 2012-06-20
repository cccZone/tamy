#include "ColorFrame.h"
#include "core\Color.h"
#include <QColorDialog.h>
#include <QColor.h>


///////////////////////////////////////////////////////////////////////////////

ColorFrame::ColorFrame( QWidget* parent, const Color& color )
: QFrame( parent )
{
   setPalette( QPalette( QColor( color.r * 255, color.g * 255, color.b * 255, color.a * 255 ) ) );
   setAutoFillBackground( true );
   setFrameStyle( QFrame::Panel | QFrame::Sunken );
   setLineWidth( 1 );
}

///////////////////////////////////////////////////////////////////////////////

ColorFrame::ColorFrame( QWidget* parent, const QColor& color )
: QFrame( parent )
{
   setPalette( QPalette( color ) );
   setAutoFillBackground( true );
   setFrameStyle( QFrame::Panel | QFrame::Sunken );
   setLineWidth( 1 );
}

///////////////////////////////////////////////////////////////////////////////

void ColorFrame::mouseDoubleClickEvent ( QMouseEvent* event )
{
   QColor oldColor = palette().color( QPalette::Background );
   QColor newColor = QColorDialog::getColor( oldColor, this, "Select color" );
   setPalette( QPalette( newColor ) );

   emit changed( newColor );
}

///////////////////////////////////////////////////////////////////////////////

QColor ColorFrame::getColor() const
{
   return palette().color( QPalette::Background );
}

///////////////////////////////////////////////////////////////////////////////

Color ColorFrame::getEngineColor() const
{
   QColor color = palette().color( QPalette::Background );
   return Color( color.redF(), color.greenF(), color.blueF(), color.alphaF() );
}

///////////////////////////////////////////////////////////////////////////////

void ColorFrame::setColor( const Color& color )
{
   QColor newColor( color.r * 255, color.g * 255, color.b * 255, color.a * 255 );
   setPalette( QPalette( newColor ) );
   emit changed( newColor );
}

///////////////////////////////////////////////////////////////////////////////
