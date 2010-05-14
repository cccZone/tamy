#include "ColorPropertyEditor.h"
#include "core\Color.h"
#include <QBoxLayout.h>
#include <QLabel.h>
#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

ColorPropertyEditor::ColorPropertyEditor( Color& property, const std::string& label )
: QPropertyEditor( label.c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::setupUi()
{
   QFrame* frame = new QFrame(this); addWidget(frame);
   QHBoxLayout* layout = new QHBoxLayout(frame);

   layout->addWidget(new QLabel("R:", this));
   QDoubleSpinBox* rVal = new QDoubleSpinBox(this); layout->addWidget(rVal);

   layout->addWidget(new QLabel("G:", this));
   QDoubleSpinBox* gVal = new QDoubleSpinBox(this); layout->addWidget(gVal);

   layout->addWidget(new QLabel("B:", this));
   QDoubleSpinBox* bVal = new QDoubleSpinBox(this); layout->addWidget(bVal);

   layout->addWidget(new QLabel("A:", this));
   QDoubleSpinBox* aVal = new QDoubleSpinBox(this); layout->addWidget(aVal);

   rVal->setValue(m_property.r);
   gVal->setValue(m_property.g);
   bVal->setValue(m_property.b);
   aVal->setValue(m_property.a);

   connect(rVal, SIGNAL(valueChanged(double)), this, SLOT(rValChanged(double)));
   connect(gVal, SIGNAL(valueChanged(double)), this, SLOT(gValChanged(double)));
   connect(bVal, SIGNAL(valueChanged(double)), this, SLOT(bValChanged(double)));
   connect(aVal, SIGNAL(valueChanged(double)), this, SLOT(aValChanged(double)));
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::rValChanged(double val)
{
   m_property.r = val;
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::gValChanged(double val)
{
   m_property.g = val;
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::bValChanged(double val)
{
   m_property.b = val;
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::aValChanged(double val)
{
   m_property.a = val;
}

///////////////////////////////////////////////////////////////////////////////
