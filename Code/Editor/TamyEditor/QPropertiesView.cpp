#include "QPropertiesView.h"
#include "core\Object.h"
#include "core\Color.h"
#include <QWidget.h>
#include <QScrollArea.h>
#include <QBoxLayout.h>
#include <QLabel.h>
#include <QtGui\QSpacerItem>
#include "core-MVC\Entity.h"
#include "core\Resource.h"
#include <QToolBox.h>
#include <QCommonStyle.h>
#include <QStyleOption.h>

// properties
#include "Vec3PropertyEditor.h"
#include "MtxPropertyEditor.h"
#include "StringPropertyEditor.h"
#include "SurfacePropertiesPropertyEditor.h"
#include "FloatPropertyEditor.h"
#include "IntPropertyEditor.h"
#include "BoolPropertyEditor.h"
#include "ResourcePropertyEditor.h"
#include "ArrayPropertyEditor.h"
#include "EnumPropertyEditor.h"
#include "ObjectPropertyEditor.h"

///////////////////////////////////////////////////////////////////////////////

QPropertiesView::QPropertiesView()
: QPropertyEditor("")
, m_toolBox(NULL)
{
   m_toolBox = new QToolBox( this );
   m_toolBox->setStyleSheet( "QToolBox::tab {                                 \
      background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,                 \
      stop: 0 #60C7FF, stop: 1.0 #149CE0);                                    \
         border-radius: 5px;                                                  \
      color: black;                                                           \
      }                                                                       \
      QToolBox::tab:selected {                                                \
      background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,                 \
      stop: 0 #149CE0, stop: 1.0 #60C7FF);                                    \
      font: italic;                                                           \
      color: black;                                                           \
      }" );

   addWidget( m_toolBox );

   initFactory();
}

///////////////////////////////////////////////////////////////////////////////

QPropertiesView::~QPropertiesView()
{
   m_toolBox = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::initFactory()
{
   associate< D3DXVECTOR3,         Vec3PropertyEditor >();
   associate< D3DXMATRIX,          MtxPropertyEditor >();
   associate< std::string,         StringPropertyEditor >();
   associate< SurfaceProperties,   SurfacePropertiesPropertyEditor >();
   associate< bool,                BoolPropertyEditor >();
   associate< float,               FloatPropertyEditor >();
   associate< double,              DoublePropertyEditor >();
   associate< int,                 IntPropertyEditor >();
   associate< unsigned int,        UIntPropertyEditor >();
   associate< long,                LongPropertyEditor >();
   associate< unsigned long,       ULongPropertyEditor >();
   associatePtr< Resource*,        ResourcePropertyEditor >();
   associatePtr< Object*,           ObjectPropertyEditor >();
   associatePtr< Enum,             EnumPropertyEditor >();
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::addPropertyEditor( QWidget* editorWidget )
{
   editorWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
   m_currWidget->addWidget( editorWidget );
   m_toolBox->setCurrentIndex( m_toolBox->count() - 1 );
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::removePropertyEditor( QWidget& editorWidget )
{
   m_currWidget->removeWidget( &editorWidget );
   m_toolBox->setCurrentIndex( m_toolBox->count() - 1 );
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::onSet( Properties& properties )
{
   QFrame* frame = new QFrame( this );
   frame->setLayout( m_currWidget = new QVBoxLayout( frame ) );
   m_currWidget->setSpacing(0);
   m_currWidget->setMargin(0);

   m_toolBox->addItem( frame, properties.getClassName().c_str() );
}

///////////////////////////////////////////////////////////////////////////////
