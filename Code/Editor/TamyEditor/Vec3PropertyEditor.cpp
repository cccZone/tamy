#include "Vec3PropertyEditor.h"
#include <QBoxLayout.h>
#include <QLabel.h>
#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

Vec3PropertyEditor::Vec3PropertyEditor( TEditableReflectionProperty< Vector >* property )
: QPropertyEditor( property->getLabel().c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

Vec3PropertyEditor::~Vec3PropertyEditor()
{
   delete m_property;
   m_property = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Vec3PropertyEditor::setupUi()
{
   QFrame* frame = new QFrame(this); addWidget(frame);
   QHBoxLayout* layout = new QHBoxLayout(frame);
   layout->setSpacing( 0 );
   layout->setMargin( 0 );

   layout->addWidget(new QLabel("X:", this));
   QDoubleSpinBox* xVal = new QDoubleSpinBox(this); layout->addWidget(xVal);
   xVal->setRange( -FLT_MAX, FLT_MAX );

   layout->addWidget(new QLabel("Y:", this));
   QDoubleSpinBox* yVal = new QDoubleSpinBox(this); layout->addWidget(yVal);
   yVal->setRange( -FLT_MAX, FLT_MAX );

   layout->addWidget(new QLabel("Z:", this));
   QDoubleSpinBox* zVal = new QDoubleSpinBox(this); layout->addWidget(zVal);
   zVal->setRange( -FLT_MAX, FLT_MAX );

   const Vector& currVal = m_property->get();
   xVal->setValue(currVal.x);
   yVal->setValue(currVal.y);
   zVal->setValue(currVal.z);

   connect(xVal, SIGNAL(valueChanged(double)), this, SLOT(xValChanged(double)));
   connect(yVal, SIGNAL(valueChanged(double)), this, SLOT(yValChanged(double)));
   connect(zVal, SIGNAL(valueChanged(double)), this, SLOT(zValChanged(double)));
}

///////////////////////////////////////////////////////////////////////////////

void Vec3PropertyEditor::xValChanged(double val)
{
   Vector currVal = m_property->get();
   currVal.x = val;
   m_property->set( currVal );
}

///////////////////////////////////////////////////////////////////////////////

void Vec3PropertyEditor::yValChanged(double val)
{
   Vector currVal = m_property->get();
   currVal.y = val;
   m_property->set( currVal );
}

///////////////////////////////////////////////////////////////////////////////

void Vec3PropertyEditor::zValChanged(double val)
{
   Vector currVal = m_property->get();
   currVal.z = val;
   m_property->set( currVal );
}

///////////////////////////////////////////////////////////////////////////////
