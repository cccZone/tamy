#include "ColorPropertyEditor.h"
#include "ColorFrame.h"


///////////////////////////////////////////////////////////////////////////////

ColorPropertyEditor::ColorPropertyEditor( TEditableReflectionProperty< Color >* val )
   : QPropertyEditor( val->getLabel().c_str())
   , m_val( val )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

ColorPropertyEditor::~ColorPropertyEditor()
{
   delete m_val;
   m_val = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::setupUi()
{
   m_colorPicker = new ColorFrame( this, m_val->get() ); 
   addWidget( m_colorPicker );

   connect( m_colorPicker, SIGNAL( changed( const QColor& ) ), this, SLOT( valChanged( const QColor& ) ) );
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::valChanged( const QColor& val )
{
   m_val->set( m_colorPicker->getEngineColor() );
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::onPropertyChanged()
{
   m_colorPicker->setColor( m_val->get() );
}

///////////////////////////////////////////////////////////////////////////////

