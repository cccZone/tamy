#include "ArrayPropertyEditor.h"
#include "QPropertiesView.h"


///////////////////////////////////////////////////////////////////////////////

ArrayPropertyEditor::ArrayPropertyEditor( TEditableProperty< VectorProperty >* property )
: QPropertyEditor( property->getLabel().c_str() )
, m_property( property )
{
}

///////////////////////////////////////////////////////////////////////////////

ArrayPropertyEditor::~ArrayPropertyEditor()
{
   delete m_property;
   m_property = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ArrayPropertyEditor::onInitialize()
{
   VectorProperty& array = m_property->get();
   unsigned int count = array.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      QPropertiesView* newView = new QPropertiesView( getServicesMgr() );
      addWidget( newView );
      array.viewProperties( i, *newView );
   }
}

///////////////////////////////////////////////////////////////////////////////
