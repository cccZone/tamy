#ifndef _INT_PROPERTY_EDITOR_H
#error "This file can only be included from IntPropertyEditor.h"
#else

#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

template <typename INT_TYPE>
IntPropertyEditor<INT_TYPE>::IntPropertyEditor( INT_TYPE& property, const std::string& label )
: QPropertyEditor( label.c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

template <typename INT_TYPE>
void IntPropertyEditor<INT_TYPE>::setupUi()
{
   QSpinBox* val = new QSpinBox(this); addWidget(val);

   val->setValue(m_property);

   connect(val, SIGNAL(valueChanged(int)), this, SLOT(valChanged(int)));
}

///////////////////////////////////////////////////////////////////////////////

template <typename INT_TYPE>
void IntPropertyEditor<INT_TYPE>::valChanged(int val)
{
   m_property = val;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _INT_PROPERTY_EDITOR_H
