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
#include "ColorPropertyEditor.h"
#include "FloatPropertyEditor.h"
#include "IntPropertyEditor.h"
#include "BoolPropertyEditor.h"
#include "EntityPropertyEditor.h"
#include "ResourcePropertyEditor.h"
#include "ArrayPropertyEditor.h"

///////////////////////////////////////////////////////////////////////////////

QPropertiesView::QPropertiesView( TamyEditor& mgr )
: QPropertyEditor("")
, m_mgr( mgr )
, m_toolBox(NULL)
{
   m_toolBox = new QToolBox( this );
   m_toolBox->setStyleSheet( "QToolBox::tab {                                 \
      background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,                 \
      stop: 0 #DDFC77, stop: 0.4 #DDFC77,                                     \
      stop: 0.4 #9BCE4F, stop: 1.0 #9BCE4F);                                  \
         border-radius: 5px;                                                  \
      color: black;                                                           \
      }                                                                       \
      QToolBox::tab:selected {                                                \
      background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,                 \
      stop: 0 #9BCE4F, stop: 0.4 #9BCE4F,                                     \
      stop: 0.4 #DDFC77, stop: 1.0 #DDFC77);                                  \
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
   associate< Color,               ColorPropertyEditor >();
   associate< bool,                BoolPropertyEditor >();
   associate< float,               FloatPropertyEditor< float > >();
   associate< double,              FloatPropertyEditor< double > >();
   associate< int,                 IntPropertyEditor< int > >();
   associate< unsigned int,        IntPropertyEditor< unsigned int > >();
   associate< long,                IntPropertyEditor< long > >();
   associate< unsigned long,       IntPropertyEditor< unsigned long > >();
   associatePtr< Entity*,          EntityPropertyEditor >();
   associatePtr< Resource*,        ResourcePropertyEditor >();
   associate< VectorProperty,      ArrayPropertyEditor >();
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::addPropertyEditor(QWidget* editorWidget)
{
   editorWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
   m_currWidget->addWidget( editorWidget );
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::removePropertyEditor(QWidget& editorWidget)
{
   m_currWidget->removeWidget( &editorWidget );
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::onSet(Properties& properties)
{
   QFrame* frame = new QFrame( this );
   frame->setLayout( m_currWidget = new QVBoxLayout( frame ) );
   m_currWidget->setSpacing(0);
   m_currWidget->setMargin(0);

   m_toolBox->addItem( frame, properties.getClassName().c_str() );
}

///////////////////////////////////////////////////////////////////////////////
