#include "Vec3PropertyEditor.h"
#include <QBoxLayout.h>
#include <QLabel.h>
#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

Vec3PropertyEditor::Vec3PropertyEditor( D3DXVECTOR3& property, const std::string& label )
: QPropertyEditor( label.c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

void Vec3PropertyEditor::setupUi()
{
   QFrame* frame = new QFrame(this); addWidget(frame);
   QHBoxLayout* layout = new QHBoxLayout(frame);

   layout->addWidget(new QLabel("X:", this));
   QDoubleSpinBox* xVal = new QDoubleSpinBox(this); layout->addWidget(xVal);

   layout->addWidget(new QLabel("Y:", this));
   QDoubleSpinBox* yVal = new QDoubleSpinBox(this); layout->addWidget(yVal);

   layout->addWidget(new QLabel("Z:", this));
   QDoubleSpinBox* zVal = new QDoubleSpinBox(this); layout->addWidget(zVal);

   xVal->setValue(m_property.x);
   yVal->setValue(m_property.y);
   zVal->setValue(m_property.z);

   connect(xVal, SIGNAL(valueChanged(double)), this, SLOT(xValChanged(double)));
   connect(yVal, SIGNAL(valueChanged(double)), this, SLOT(yValChanged(double)));
   connect(zVal, SIGNAL(valueChanged(double)), this, SLOT(zValChanged(double)));
}

///////////////////////////////////////////////////////////////////////////////

void Vec3PropertyEditor::xValChanged(double val)
{
   m_property.x = val;
}

///////////////////////////////////////////////////////////////////////////////

void Vec3PropertyEditor::yValChanged(double val)
{
   m_property.y = val;
}

///////////////////////////////////////////////////////////////////////////////

void Vec3PropertyEditor::zValChanged(double val)
{
   m_property.z = val;
}

///////////////////////////////////////////////////////////////////////////////
