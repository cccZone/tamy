#include "StringPropertyEditor.h"
#include <QLineEdit.h>


///////////////////////////////////////////////////////////////////////////////

StringPropertyEditor::StringPropertyEditor(TProperty<std::string>& property)
: QPropertyEditor(property.getLabel().c_str())
, m_property(property)
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

void StringPropertyEditor::setupUi()
{
   QLineEdit* edit = new QLineEdit(this);
   addWidget(edit);
   
   std::string currVal = m_property.get();
   edit->setText(currVal.c_str());

   connect(edit, SIGNAL(textChanged(const QString&)), this, SLOT(valChanged(const QString&)));
}

///////////////////////////////////////////////////////////////////////////////

void StringPropertyEditor::valChanged(const QString& val)
{
   m_property.set(val.toStdString());
}

///////////////////////////////////////////////////////////////////////////////
