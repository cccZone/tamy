#include "QPropertyEditor.h"
#include "QReflectionObjectEditor.h"
#include <QBoxLayout.h>
#include <QFrame.h>
#include <QLabel.h>


///////////////////////////////////////////////////////////////////////////////

QPropertyEditor::QPropertyEditor( const QString& propertyLabel )
{
   m_layout = new QVBoxLayout( this );
   m_layout->setSpacing(0);
   m_layout->setMargin(0);

   setFrameStyle( QFrame::StyledPanel );

   if( propertyLabel.isEmpty() == false )
   {
      QLabel* label = new QLabel( propertyLabel, this );
      label->setPalette( QPalette( qRgb( 191, 191, 191 ) ) ); // <editor_settings.todo> property name color
      label->setAutoFillBackground(true);

      label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
      m_layout->addWidget( label );
   }

   m_spacer = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Expanding );
   m_layout->addSpacerItem( m_spacer );
}

///////////////////////////////////////////////////////////////////////////////

QPropertyEditor::~QPropertyEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::initialize( ReflectionObjectEditor* parent )
{
   QReflectionObjectEditor* parentEd = static_cast< QReflectionObjectEditor* >( parent );
   parentEd->addPropertyEditor( this );
   onInitialize();
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::deinitialize( ReflectionObjectEditor* parent )
{
   QReflectionObjectEditor* parentEd = static_cast< QReflectionObjectEditor* >( parent );
   parentEd->removePropertyEditor(*this);
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::addWidget( QWidget* widget )
{
   m_layout->removeItem( m_spacer );

   widget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
   m_layout->addWidget(widget);

   m_layout->addSpacerItem( m_spacer );
}

///////////////////////////////////////////////////////////////////////////////

void QPropertyEditor::removeWidget( QWidget& widget )
{
   m_layout->removeWidget( &widget );
}

///////////////////////////////////////////////////////////////////////////////
