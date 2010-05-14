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


///////////////////////////////////////////////////////////////////////////////

QPropertiesView::QPropertiesView( TamyEditor& mgr )
: QPropertyEditor("")
, m_mgr( mgr )
, m_propertiesLayout(NULL)
{
   initFactory();
   initUI();
}

///////////////////////////////////////////////////////////////////////////////

QPropertiesView::QPropertiesView( TamyEditor& mgr, VectorProperty& property )
: QPropertyEditor(property.getLabel().c_str())
, m_mgr( mgr )
, m_propertiesLayout(NULL)
{
   initFactory();
   initUI();

   // add the contents of the property vector to this view
   unsigned int count = property.size();
   if (count == 0)
   {
      m_propertiesLayout->addWidget(new QLabel("<<no properties>>", this));
   }
   else
   {
      for (unsigned int i = 0; i < count; ++i)
      {
         Object* obj = property.get(i);
         if (obj != NULL)
         {
            obj->viewProperties(*this);
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

QPropertiesView::~QPropertiesView()
{
   m_propertiesLayout = NULL;
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
   associatePtr< Entity,           EntityPropertyEditor >();
   associatePtr< Resource,         ResourcePropertyEditor >();
   //associate< VectorProperty,      QPropertiesView >();
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::initUI()
{
  /* QWidget* viewWidget = new QWidget( this );
   addWidget(viewWidget);

   m_propertiesLayout = new QVBoxLayout( viewWidget );
   m_propertiesLayout->setSpacing(0);
   m_propertiesLayout->setMargin(1);
   viewWidget->setLayout(m_propertiesLayout);*/
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::addPropertyEditor(QWidget* editorWidget)
{
   //editorWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
   addWidget( editorWidget );
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::removePropertyEditor(QWidget& editorWidget)
{
   removeWidget( editorWidget );
   //m_propertiesLayout->removeWidget(&editorWidget);
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::onSet(Properties& properties)
{
   QLabel* classNameLabel = new QLabel(properties.getClassName().c_str(), this);
   classNameLabel->setPalette(QPalette(qRgb(53, 191, 255)));
   classNameLabel->setAutoFillBackground(true);
   addWidget( classNameLabel );
   //classNameLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
   //m_propertiesLayout->addWidget(classNameLabel);
}

///////////////////////////////////////////////////////////////////////////////
