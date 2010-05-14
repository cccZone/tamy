#include "BoolPropertyEditor.h"
#include <QCheckBox.h>


///////////////////////////////////////////////////////////////////////////////

BoolPropertyEditor::BoolPropertyEditor( TEditableProperty< bool >* val )
: QPropertyEditor( val->getLabel().c_str())
, m_val( val )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

BoolPropertyEditor::~BoolPropertyEditor()
{
   delete m_val;
   m_val = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void BoolPropertyEditor::setupUi()
{
   QCheckBox* val = new QCheckBox(this); addWidget(val);
   val->setChecked( m_val->get() );

   connect(val, SIGNAL(stateChanged(int)), this, SLOT(valChanged(int)));
}

///////////////////////////////////////////////////////////////////////////////

void BoolPropertyEditor::valChanged(int val)
{
   m_val->set( val != 0 );
}

///////////////////////////////////////////////////////////////////////////////
