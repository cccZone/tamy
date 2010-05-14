#include "BoolPropertyEditor.h"
#include <QCheckBox.h>


///////////////////////////////////////////////////////////////////////////////

BoolPropertyEditor::BoolPropertyEditor( bool& val, const std::string& label )
: QPropertyEditor( label.c_str())
, m_val( val )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

void BoolPropertyEditor::setupUi()
{
   QCheckBox* val = new QCheckBox(this); addWidget(val);
   val->setChecked( m_val );

   connect(val, SIGNAL(stateChanged(int)), this, SLOT(valChanged(int)));
}

///////////////////////////////////////////////////////////////////////////////

void BoolPropertyEditor::valChanged(int val)
{
   m_val = (val != 0);
}

///////////////////////////////////////////////////////////////////////////////
