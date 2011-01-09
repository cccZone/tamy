#include "FloatPropertyEditor.h"
#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

FloatPropertyEditor::FloatPropertyEditor( TEditableProperty< float >* property )
: QPropertyEditor( property->getLabel().c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

FloatPropertyEditor::~FloatPropertyEditor()
{
   delete m_property;
   m_property = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void FloatPropertyEditor::setupUi()
{
   QDoubleSpinBox* val = new QDoubleSpinBox(this); addWidget(val);
   val->setSingleStep( 0.1 );
   val->setRange( -FLT_MAX, FLT_MAX );
   val->setValue( m_property->get() );

   connect(val, SIGNAL(valueChanged(double)), this, SLOT(valChanged(double)));
}

///////////////////////////////////////////////////////////////////////////////

void FloatPropertyEditor::valChanged(double val)
{
   m_property->set( val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DoublePropertyEditor::DoublePropertyEditor( TEditableProperty< double >* property )
   : QPropertyEditor( property->getLabel().c_str() )
   , m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

DoublePropertyEditor::~DoublePropertyEditor()
{
   delete m_property;
   m_property = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DoublePropertyEditor::setupUi()
{
   QDoubleSpinBox* val = new QDoubleSpinBox(this); addWidget(val);
   val->setSingleStep( 0.1 );
   val->setRange( -FLT_MAX, FLT_MAX );
   val->setValue( m_property->get() );

   connect(val, SIGNAL(valueChanged(double)), this, SLOT(valChanged(double)));
}

///////////////////////////////////////////////////////////////////////////////

void DoublePropertyEditor::valChanged(double val)
{
   m_property->set( val );
}

///////////////////////////////////////////////////////////////////////////////