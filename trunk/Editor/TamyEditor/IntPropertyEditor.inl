#ifndef _INT_PROPERTY_EDITOR_H
#error "This file can only be included from IntPropertyEditor.h"
#else

#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

template <typename INT_TYPE>
IntPropertyEditor<INT_TYPE>::IntPropertyEditor(TProperty<INT_TYPE>& property)
: QPropertyEditor(property.getLabel().c_str())
, m_property(property)
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

template <typename INT_TYPE>
void IntPropertyEditor<INT_TYPE>::setupUi()
{
   QSpinBox* val = new QSpinBox(this); addWidget(val);

   INT_TYPE currVal = m_property.get();
   val->setValue(currVal);

   connect(val, SIGNAL(valueChanged(int)), this, SLOT(valChanged(int)));
}

///////////////////////////////////////////////////////////////////////////////

template <typename INT_TYPE>
void IntPropertyEditor<INT_TYPE>::valChanged(int val)
{
   m_property.set(val);
}

///////////////////////////////////////////////////////////////////////////////

#endif // _INT_PROPERTY_EDITOR_H
