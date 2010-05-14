#include "EntityPropertyEditor.h"
#include <QLabel>


///////////////////////////////////////////////////////////////////////////////

EntityPropertyEditor::EntityPropertyEditor( TEditableProperty< Entity* >* property )
: QPropertyEditor( property->getLabel().c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

EntityPropertyEditor::~EntityPropertyEditor()
{
   delete m_property;
   m_property = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void EntityPropertyEditor::setupUi()
{
   addWidget(new QLabel("Entity pointer editor", this));
}

///////////////////////////////////////////////////////////////////////////////

