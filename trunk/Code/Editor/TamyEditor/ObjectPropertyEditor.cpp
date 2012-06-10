#include "ObjectPropertyEditor.h"
#include "core.h"
#include "tamyeditor.h"
#include <QComboBox>
#include "QPropertiesView.h"


///////////////////////////////////////////////////////////////////////////////

ObjectPropertyEditor::ObjectPropertyEditor( TEditableReflectionProperty< ReflectionObject* >* property )
   : QPropertyEditor( property->getLabel().c_str() )
   , m_property( property )
   , m_typeSelectionBox( NULL )
   , m_layout( NULL )
   , m_propertiesView( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

ObjectPropertyEditor::~ObjectPropertyEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void ObjectPropertyEditor::onInitialize()
{
   QWidget* frame = new QWidget( this );
   m_layout = new QVBoxLayout( frame );
   m_layout->setSpacing(0);
   m_layout->setMargin(0);
   frame->setLayout( m_layout );
   addWidget( frame );

   // a combo box that allows to select object type
   {
      m_typeSelectionBox = new QComboBox( frame );
      m_typeSelectionBox->setMinimumSize( 20, 20 );
      m_typeSelectionBox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
      m_layout->addWidget( m_typeSelectionBox );

      // fill the combo box with choices
      {
         // add an empty string that when selected will be interpreted as desire to delete
         // the currently set instance and set NULL pointer instead
         m_typeSelectionBox->addItem( "" );

         // add the selected class, if it's not abstract of course
         const SerializableReflectionType& propertyClass = static_cast< const SerializableReflectionType& >( m_property->getType() );
         if ( !propertyClass.isAbstract() )
         {
            m_typeSelectionBox->addItem( propertyClass.m_name.c_str() );
         }

         // and finally add the names of the non-abstract implementations of the property type
         std::vector< const SerializableReflectionType* > implementations;
         propertyClass.collectImplementations( implementations, false );

         unsigned int count = implementations.size();
         for ( unsigned int i = 0; i < count; ++i )
         {
            m_typeSelectionBox->addItem( implementations[i]->m_name.c_str() );
         }
      }

      connect( m_typeSelectionBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( valChanged( const QString& ) ) );

      // set the proper name of the object's class
      ReflectionObject* currObj = m_property->get();
      if ( currObj )
      {
         const SerializableReflectionType& currClass = currObj->getVirtualRTTI();
         int itemIdx = m_typeSelectionBox->findText( currClass.m_name.c_str() );
         m_typeSelectionBox->setCurrentIndex( itemIdx );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ObjectPropertyEditor::refresh()
{
   // get the selected class name
   QString newClassName = m_typeSelectionBox->currentText();
   ReflectionObject* currObj = m_property->get();
   QString currClassName = currObj ? currObj->getVirtualRTTI().m_name.c_str() : "";

   if ( currClassName != newClassName )
   { 
      // delete the previous object - it will change
      delete currObj;

      if ( newClassName.isEmpty() )
      {
         // the user wants to set NULL pointer
         m_property->set( NULL );
      }
      else
      {
         // create an instance of the newly selected class
         SerializableReflectionType* newClass = ReflectionTypesRegistry::getInstance().findSerializable( newClassName.toStdString() );
         ASSERT_MSG( newClass != NULL, "The selected type is invalid" );
         if ( newClass != NULL )
         {
            ReflectionObject* newInstance = newClass->instantiate< ReflectionObject >();
            m_property->set( newInstance );
         }
      }
   }

   // and now - if there's a new object set, edit its properties
   {
      // delete the previous properties view
      if ( m_propertiesView )
      {
         m_layout->removeWidget( m_propertiesView );
         delete m_propertiesView;
         m_propertiesView = NULL;
      }

      // and, if an object was created, create a view of its properties
      currObj = m_property->get();
      if ( currObj )
      {
         // remove the old properties view, if there was any
         m_propertiesView = new QPropertiesView();
         m_layout->addWidget( m_propertiesView, 1 );
         currObj->viewProperties( *m_propertiesView );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ObjectPropertyEditor::valChanged( const QString& text )
{
   refresh();
}

///////////////////////////////////////////////////////////////////////////////
