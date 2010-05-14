#include "ColorPropertyEditor.h"
#include "core\Color.h"
#include <QBoxLayout.h>
#include <QLabel.h>
#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

ColorPropertyEditor::ColorPropertyEditor( TEditableProperty< Color >* property )
: QPropertyEditor( property->getLabel().c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

ColorPropertyEditor::~ColorPropertyEditor()
{
   delete m_property;
   m_property = NULL;
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

   const Color& val = m_property->get();
   rVal->setValue(val.r);
   gVal->setValue(val.g);
   bVal->setValue(val.b);
   aVal->setValue(val.a);

   connect(rVal, SIGNAL(valueChanged(double)), this, SLOT(rValChanged(double)));
   connect(gVal, SIGNAL(valueChanged(double)), this, SLOT(gValChanged(double)));
   connect(bVal, SIGNAL(valueChanged(double)), this, SLOT(bValChanged(double)));
   connect(aVal, SIGNAL(valueChanged(double)), this, SLOT(aValChanged(double)));
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::rValChanged(double val)
{
   Color newVal = m_property->get();
   newVal.r = val;
   m_property->set( newVal );
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::gValChanged(double val)
{
   Color newVal = m_property->get();
   newVal.g = val;
   m_property->set( newVal );
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::bValChanged(double val)
{
   Color newVal = m_property->get();
   newVal.b = val;
   m_property->set( newVal );
}

///////////////////////////////////////////////////////////////////////////////

void ColorPropertyEditor::aValChanged(double val)
{
   Color newVal = m_property->get();
   newVal.a = val;
   m_property->set( newVal );
}

///////////////////////////////////////////////////////////////////////////////
