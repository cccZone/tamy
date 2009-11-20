#include "QPropertyEditor.h"
#include "PropertiesEditor.h"
#include <QBoxLayout.h>
#include <QFrame.h>
#include <QLabel.h>


///////////////////////////////////////////////////////////////////////////////

QPropertyEditor::QPropertyEditor(const QString& propertyLabel)
{
   m_layout = new QVBoxLayout(this);
   setFrameStyle(QFrame::StyledPanel);

   m_layout->addWidget(new QLabel(propertyLabel, this));
}

///////////////////////////////////////////////////////////////////////////////

QPropertyEditor::~QPropertyEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::initialize(PropertiesEditor& view)
{
   view.addPropertyEditor(this);
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::deinitialize(PropertiesEditor& view)
{
   view.removePropertyEditor(*this);
}


///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::addWidget(QWidget* widget)
{
   m_layout->addWidget(widget);
}

///////////////////////////////////////////////////////////////////////////////
