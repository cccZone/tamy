#include "Vec3PropertyEditor.h"
#include <QBoxLayout.h>
#include <QLabel.h>
#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

Vec3PropertyEditor::Vec3PropertyEditor(TProperty<D3DXVECTOR3>& property)
: QPropertyEditor(property.getLabel().c_str())
, m_property(property)
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

   D3DXVECTOR3 currVal = m_property.get();
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
   D3DXVECTOR3 newVal = m_property.get();
   newVal.x = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void Vec3PropertyEditor::yValChanged(double val)
{
   D3DXVECTOR3 newVal = m_property.get();
   newVal.y = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void Vec3PropertyEditor::zValChanged(double val)
{
   D3DXVECTOR3 newVal = m_property.get();
   newVal.z = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////
