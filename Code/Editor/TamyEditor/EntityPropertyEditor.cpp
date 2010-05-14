#include "EntityPropertyEditor.h"
#include <QLabel>


///////////////////////////////////////////////////////////////////////////////

EntityPropertyEditor::EntityPropertyEditor( Entity*& property, 
                                            const Class& acceptableType, 
                                            const std::string& label )
: QPropertyEditor( label.c_str() )
, m_property( property )
, m_acceptableType( acceptableType )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

void EntityPropertyEditor::setupUi()
{
   addWidget(new QLabel("Entity pointer editor", this));
}

///////////////////////////////////////////////////////////////////////////////

