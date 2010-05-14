#include "ResourcePropertyEditor.h"
#include <QLabel>


///////////////////////////////////////////////////////////////////////////////

ResourcePropertyEditor::ResourcePropertyEditor( Resource*& property, const std::string& label )
: QPropertyEditor( label.c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcePropertyEditor::setupUi()
{
   addWidget( new QLabel( "Resource:", this ) );
}

///////////////////////////////////////////////////////////////////////////////
