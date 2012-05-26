#include "QPropertiesView.h"
#include "core\ReflectionObject.h"
#include <QWidget.h>
#include <QBoxLayout.h>
#include <QtGui\QSpacerItem>

// properties
#include "Vec3PropertyEditor.h"
#include "MtxPropertyEditor.h"
#include "StringPropertyEditor.h"
#include "SurfacePropertiesPropertyEditor.h"
#include "FloatPropertyEditor.h"
#include "IntPropertyEditor.h"
#include "BoolPropertyEditor.h"
#include "ResourcePropertyEditor.h"
#include "EnumPropertyEditor.h"
#include "ObjectPropertyEditor.h"
#include "QReflectionPropertyEditorComposite.h"
#include "QReflectionObjectEditor.h"


///////////////////////////////////////////////////////////////////////////////

QPropertiesView::QPropertiesView()
{
   // setup the UI
   m_layout = new QVBoxLayout( this );
   m_layout->setSpacing(0);
   m_layout->setMargin(0);

   setFrameStyle( QFrame::StyledPanel );

   // setup editor factories
   initFactory();
}

///////////////////////////////////////////////////////////////////////////////

QPropertiesView::~QPropertiesView()
{
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::initFactory()
{
   associate< Vector,               Vec3PropertyEditor >();
   associate< Matrix,               MtxPropertyEditor >();
   associate< std::string,          StringPropertyEditor >();
   associate< SurfaceProperties,    SurfacePropertiesPropertyEditor >();
   associate< bool,                 BoolPropertyEditor >();
   associate< float,                FloatPropertyEditor >();
   associate< double,               DoublePropertyEditor >();
   associate< int,                  IntPropertyEditor >();
   associate< unsigned int,         UIntPropertyEditor >();
   associate< long,                 LongPropertyEditor >();
   associate< unsigned long,        ULongPropertyEditor >();
   associateAbstract< Resource*,         ResourcePropertyEditor >();
   associateAbstract< ReflectionObject*, ObjectPropertyEditor >();
   associateEnums< EnumPropertyEditor >();
   associateArray< QReflectionPropertyEditorComposite >();
   associateObjectNode< QReflectionObjectEditor >();
}


///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::onSet( ReflectionObjectEditor* rootEditor )
{
   QReflectionObjectEditor* arrRootEditor = static_cast< QReflectionObjectEditor* >( rootEditor );
   m_layout->addWidget( arrRootEditor );

   QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Expanding );
   m_layout->addSpacerItem( spacer );
}

///////////////////////////////////////////////////////////////////////////////
