#include "EnumPropertyEditor.h"
#include <QComboBox>


///////////////////////////////////////////////////////////////////////////////

EnumPropertyEditor::EnumPropertyEditor( TEditableProperty< Enum >* property )
   : QPropertyEditor( property->getLabel().c_str() )
   , m_property( property )
{
   Class enumType = m_property->getType();
   m_enumeration = enumType.instantiate< Enum >();

   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

EnumPropertyEditor::~EnumPropertyEditor()
{
   delete m_property;
   m_property = NULL;

   delete m_enumeration;
   m_enumeration = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void EnumPropertyEditor::setupUi()
{
   // create the edit widget
   QComboBox* edit = new QComboBox( this );
   addWidget( edit );

   // fill the widget with initial data
   std::vector< std::string > enumerators;
   m_enumeration->getEnumerators( enumerators );
   for ( std::vector< std::string >::const_iterator it = enumerators.begin(); it != enumerators.end(); ++it )
   {
      edit->addItem( it->c_str() );
   }

   // select the currently held value
   int val = reinterpret_cast< const int& >( m_property->get() );
   unsigned  int idx = m_enumeration->getIndex( val );
   edit->setCurrentIndex( idx );
   
   connect( edit, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( valChanged( const QString& ) ) );
}

///////////////////////////////////////////////////////////////////////////////

void EnumPropertyEditor::valChanged( const QString& val )
{
   std::string newVal = val.toStdString();
   
   int newEnumVal = m_enumeration->getValue( newVal );
   m_property->set( reinterpret_cast< const Enum& >( newEnumVal ) );
}

///////////////////////////////////////////////////////////////////////////////
