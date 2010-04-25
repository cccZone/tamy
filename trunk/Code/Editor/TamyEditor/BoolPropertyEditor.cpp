#include "BoolPropertyEditor.h"
#include <QCheckBox.h>


///////////////////////////////////////////////////////////////////////////////

BoolPropertyEditor::BoolPropertyEditor(TProperty<bool>& property)
: QPropertyEditor(property.getLabel().c_str())
, m_property(property)
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

void BoolPropertyEditor::setupUi()
{
   QCheckBox* val = new QCheckBox(this); addWidget(val);
   val->setChecked(m_property.get());

   connect(val, SIGNAL(stateChanged(int)), this, SLOT(valChanged(int)));
}

///////////////////////////////////////////////////////////////////////////////

void BoolPropertyEditor::valChanged(int val)
{
   m_property.set(val != 0);
}

///////////////////////////////////////////////////////////////////////////////
