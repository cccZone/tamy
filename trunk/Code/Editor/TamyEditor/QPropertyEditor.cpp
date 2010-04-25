#include "QPropertyEditor.h"
#include "QPropertiesView.h"
#include <QBoxLayout.h>
#include <QFrame.h>
#include <QLabel.h>


///////////////////////////////////////////////////////////////////////////////

QPropertyEditor::QPropertyEditor(const QString& propertyLabel)
{
   m_layout = new QVBoxLayout(this);
   m_layout->setSpacing(0);
   m_layout->setMargin(0);

   setFrameStyle(QFrame::StyledPanel);

   if (propertyLabel.isEmpty() == false)
   {
      m_layout->addWidget(new QLabel(propertyLabel, this));
   }
}

///////////////////////////////////////////////////////////////////////////////

QPropertyEditor::~QPropertyEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::initialize(QPropertiesView& view)
{
   view.addPropertyEditor(this);
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::deinitialize(QPropertiesView& view)
{
   view.removePropertyEditor(*this);
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::addWidget(QWidget* widget)
{
   m_layout->addWidget(widget);
}

///////////////////////////////////////////////////////////////////////////////
