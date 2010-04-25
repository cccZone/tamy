#ifndef _FLOAT_PROPERTY_EDITOR_H
#error "This file can only be included from FloatPropertyEditor.h"
#else

#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

template <typename FLOAT_TYPE>
FloatPropertyEditor<FLOAT_TYPE>::FloatPropertyEditor(TProperty<FLOAT_TYPE>& property)
: QPropertyEditor(property.getLabel().c_str())
, m_property(property)
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

template <typename FLOAT_TYPE>
void FloatPropertyEditor<FLOAT_TYPE>::setupUi()
{
   QDoubleSpinBox* val = new QDoubleSpinBox(this); addWidget(val);

   FLOAT_TYPE currVal = m_property.get();
   val->setValue(currVal);

   connect(val, SIGNAL(valueChanged(double)), this, SLOT(valChanged(double)));
}

///////////////////////////////////////////////////////////////////////////////

template <typename FLOAT_TYPE>
void FloatPropertyEditor<FLOAT_TYPE>::valChanged(double val)
{
   m_property.set(val);
}

///////////////////////////////////////////////////////////////////////////////

#endif // _FLOAT_PROPERTY_EDITOR_H
