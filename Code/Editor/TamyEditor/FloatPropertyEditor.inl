#ifndef _FLOAT_PROPERTY_EDITOR_H
#error "This file can only be included from FloatPropertyEditor.h"
#else

#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

template <typename FLOAT_TYPE>
FloatPropertyEditor<FLOAT_TYPE>::FloatPropertyEditor( TEditableProperty< FLOAT_TYPE >* property )
: QPropertyEditor( property->getLabel().c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

template <typename FLOAT_TYPE>
FloatPropertyEditor<FLOAT_TYPE>::~FloatPropertyEditor()
{
   delete m_property;
   m_property = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template <typename FLOAT_TYPE>
void FloatPropertyEditor<FLOAT_TYPE>::setupUi()
{
   QDoubleSpinBox* val = new QDoubleSpinBox(this); addWidget(val);
   val->setRange( -FLT_MAX, FLT_MAX );
   val->setValue( m_property->get() );

   connect(val, SIGNAL(valueChanged(double)), this, SLOT(valChanged(double)));
}

///////////////////////////////////////////////////////////////////////////////

template <typename FLOAT_TYPE>
void FloatPropertyEditor<FLOAT_TYPE>::valChanged(double val)
{
   m_property->set( val );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _FLOAT_PROPERTY_EDITOR_H
