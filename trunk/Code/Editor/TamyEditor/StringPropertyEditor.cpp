#include "StringPropertyEditor.h"
#include <QLineEdit.h>


///////////////////////////////////////////////////////////////////////////////

StringPropertyEditor::StringPropertyEditor( std::string& property, const std::string& label )
: QPropertyEditor( label.c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

void StringPropertyEditor::setupUi()
{
   QLineEdit* edit = new QLineEdit(this);
   addWidget(edit);
   
   edit->setText(m_property.c_str());

   connect(edit, SIGNAL(textChanged(const QString&)), this, SLOT(valChanged(const QString&)));
}

///////////////////////////////////////////////////////////////////////////////

void StringPropertyEditor::valChanged(const QString& val)
{
   m_property = val.toStdString();
}

///////////////////////////////////////////////////////////////////////////////
