#include "QReflectionObjectEditor.h"
#include "QReflectionPropertyEditorComposite.h"
#include "core/ReflectionObject.h"
#include <QVBoxLayout>
#include <QtGui\QSpacerItem>
#include <QLabel>


///////////////////////////////////////////////////////////////////////////////

QReflectionObjectEditor::QReflectionObjectEditor( ReflectionObject* editedObject )
   : ReflectionObjectEditor( editedObject )
{
   m_layout = new QVBoxLayout( this );
   m_layout->setSpacing(0);
   m_layout->setMargin(0);

   setFrameStyle( QFrame::StyledPanel );

   // set the caption with the edited object's class name
   QString propertyLabel( editedObject->getVirtualRTTI().m_name.c_str() );
   if ( propertyLabel.isEmpty() == false )
   {
      QLabel* label = new QLabel( propertyLabel, this );
      label->setPalette( QPalette( qRgb( 112, 96, 61 ) ) );  // <editor_settings.todo> object type name color
      label->setAutoFillBackground( true );

      label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
      m_layout->addWidget(label);
   }

   m_spacer = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Expanding );
   m_layout->addSpacerItem( m_spacer );
}

///////////////////////////////////////////////////////////////////////////////

QReflectionObjectEditor::~QReflectionObjectEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void QReflectionObjectEditor::initialize( ReflectionPropertyEditorComposite* parentNode )
{
   if ( parentNode )
   {
      QReflectionPropertyEditorComposite* qEditor  = static_cast< QReflectionPropertyEditorComposite* >( parentNode );
      qEditor->addPropertyEditor( this );
   }

   __super::initialize( parentNode );
}

///////////////////////////////////////////////////////////////////////////////

void QReflectionObjectEditor::deinitialize( ReflectionPropertyEditorComposite* parentNode )
{
   __super::deinitialize( parentNode );
}

///////////////////////////////////////////////////////////////////////////////

void QReflectionObjectEditor::addPropertyEditor( QWidget* editorWidget )
{
   m_layout->removeItem( m_spacer );

   editorWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
   m_layout->addWidget(editorWidget);

   m_layout->addSpacerItem( m_spacer );

}

///////////////////////////////////////////////////////////////////////////////

void QReflectionObjectEditor::removePropertyEditor( QWidget& editorWidget )
{
   m_layout->removeWidget( &editorWidget );

}

///////////////////////////////////////////////////////////////////////////////
