#include "StringPropertyEditor.h"
#include <QLineEdit.h>


///////////////////////////////////////////////////////////////////////////////

StringPropertyEditor::StringPropertyEditor( TEditableReflectionProperty< std::string >* property )
: QPropertyEditor( property->getLabel().c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

StringPropertyEditor::~StringPropertyEditor()
{
   delete m_property;
   m_property = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void StringPropertyEditor::setupUi()
{
   m_edit = new QLineEdit(this);
   addWidget(m_edit);
   
   m_edit->setText(m_property->get().c_str());

   connect(m_edit, SIGNAL(textChanged(const QString&)), this, SLOT(valChanged(const QString&)));
}

///////////////////////////////////////////////////////////////////////////////

void StringPropertyEditor::valChanged(const QString& val)
{
   std::string newVal = val.toStdString();
   m_property->set( newVal );
}

///////////////////////////////////////////////////////////////////////////////

void StringPropertyEditor::onPropertyChanged()
{
   m_edit->setText(m_property->get().c_str());
}

///////////////////////////////////////////////////////////////////////////////
