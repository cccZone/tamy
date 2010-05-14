#include "QPropertyEditor.h"
#include "QPropertiesView.h"
#include <QBoxLayout.h>
#include <QFrame.h>
#include <QLabel.h>


///////////////////////////////////////////////////////////////////////////////

QPropertyEditor::QPropertyEditor(const QString& propertyLabel)
: m_mgr( NULL )
{
   m_layout = new QVBoxLayout( this );
   m_layout->setSpacing(0);
   m_layout->setMargin(0);

   setFrameStyle(QFrame::StyledPanel);

   if (propertyLabel.isEmpty() == false)
   {
      QLabel* label = new QLabel(propertyLabel, this );
      label->setPalette(QPalette(qRgb(191, 191, 191)));
      label->setAutoFillBackground(true);

      label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
      m_layout->addWidget(label);
   }

   m_spacer = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Expanding );
   m_layout->addSpacerItem( m_spacer );
}

///////////////////////////////////////////////////////////////////////////////

QPropertyEditor::~QPropertyEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::initialize( QPropertiesView& view )
{
   m_mgr = &view.getServicesMgr();
   view.addPropertyEditor(this);
   onInitialize();
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::deinitialize( QPropertiesView& view )
{
   view.removePropertyEditor(*this);
   m_mgr = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::addWidget(QWidget* widget)
{
   m_layout->removeItem( m_spacer );

   widget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
   m_layout->addWidget(widget);

   m_layout->addSpacerItem( m_spacer );
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::removeWidget(QWidget& widget)
{
   m_layout->removeWidget(&widget);
}

///////////////////////////////////////////////////////////////////////////////
