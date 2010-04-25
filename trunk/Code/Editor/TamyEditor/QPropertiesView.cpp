#include "QPropertiesView.h"
#include "core\Object.h"
#include "core\Color.h"
#include <QWidget.h>
#include <QScrollArea.h>
#include <QBoxLayout.h>
#include <QLabel.h>
#include <QtGui\QSpacerItem>
#include "core-MVC\Entity.h"

// properties
#include "Vec3PropertyEditor.h"
#include "MtxPropertyEditor.h"
#include "StringPropertyEditor.h"
#include "ColorPropertyEditor.h"
#include "FloatPropertyEditor.h"
#include "IntPropertyEditor.h"
#include "BoolPropertyEditor.h"
#include "EntityPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

QPropertiesView::QPropertiesView()
: QPropertyEditor("")
, m_propertiesLayout(NULL)
{
   initFactory();
   initUI();
}

///////////////////////////////////////////////////////////////////////////////

QPropertiesView::QPropertiesView(VectorProperty& property)
: QPropertyEditor(property.getLabel().c_str())
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
   associate<TProperty<D3DXVECTOR3>, Vec3PropertyEditor>();
   associate<TProperty<D3DXMATRIX>, MtxPropertyEditor>();
   associate<TProperty<std::string>, StringPropertyEditor>();
   associate<TProperty<Color>, ColorPropertyEditor>();
   associate<TProperty<bool>, BoolPropertyEditor>();
   associate<TProperty<float>, FloatPropertyEditor<float> >();
   associate<TProperty<double>, FloatPropertyEditor<double> >();
   associate<TProperty<int>, IntPropertyEditor<int> >();
   associate<TProperty<unsigned int>, IntPropertyEditor<unsigned int> >();
   associate<TProperty<long>, IntPropertyEditor<long> >();
   associate<TProperty<unsigned long>, IntPropertyEditor<unsigned long> >();
   associateAbstract<TProperty<Entity*>, EntityPropertyEditor>();
   associateAbstract<VectorProperty, QPropertiesView>();
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::initUI()
{
   QScrollArea* scrollArea = new QScrollArea(this); addWidget(scrollArea);
   scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
   scrollArea->setWidgetResizable(true);

   m_propertiesLayout = new QVBoxLayout(scrollArea);
   m_propertiesLayout->setSpacing(0);
   m_propertiesLayout->setMargin(1);
   m_propertiesLayout->setObjectName(QString::fromUtf8("propertiesLayout"));
   scrollArea->setLayout(m_propertiesLayout);

   m_spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::addPropertyEditor(QWidget* editorWidget)
{
   m_propertiesLayout->removeItem(m_spacer);
   m_propertiesLayout->addWidget(editorWidget);
   m_propertiesLayout->addItem(m_spacer);
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::removePropertyEditor(QWidget& editorWidget)
{
   m_propertiesLayout->removeWidget(&editorWidget);
}

///////////////////////////////////////////////////////////////////////////////

void QPropertiesView::onSet(Properties& properties)
{
   QLabel* classNameLabel = new QLabel(properties.getClassName().c_str(), this);
   classNameLabel->setPalette(QPalette(qRgb(53, 191, 255)));
   classNameLabel->setAutoFillBackground(true);
   m_propertiesLayout->addWidget(classNameLabel);
}

///////////////////////////////////////////////////////////////////////////////
