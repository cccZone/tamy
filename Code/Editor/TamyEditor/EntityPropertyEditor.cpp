#include "EntityPropertyEditor.h"
#include <QLabel>


///////////////////////////////////////////////////////////////////////////////

EntityPropertyEditor::EntityPropertyEditor( Entity*& property, const std::string& label )
: QPropertyEditor( label.c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

void EntityPropertyEditor::setupUi()
{
   addWidget(new QLabel("Entity pointer editor", this));
}

///////////////////////////////////////////////////////////////////////////////

