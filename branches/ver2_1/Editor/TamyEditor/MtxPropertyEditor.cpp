#include "MtxPropertyEditor.h"
#include <QFrame.h>
#include <QBoxLayout.h>
#include <QLabel.h>
#include <QSpinBox.h>


///////////////////////////////////////////////////////////////////////////////

MtxPropertyEditor::MtxPropertyEditor(TProperty<D3DXMATRIX>& property)
: QPropertyEditor(property.getLabel().c_str())
, m_property(property)
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::setupUi()
{
   QFrame* rightRow = new QFrame(this); addWidget(rightRow);
   QHBoxLayout* rightRowLayout = new QHBoxLayout(rightRow);
   rightRowLayout->addWidget(new QLabel("right"));
   QDoubleSpinBox* m11Val = new QDoubleSpinBox(rightRow); rightRowLayout->addWidget(m11Val);
   QDoubleSpinBox* m12Val = new QDoubleSpinBox(rightRow); rightRowLayout->addWidget(m12Val);
   QDoubleSpinBox* m13Val = new QDoubleSpinBox(rightRow); rightRowLayout->addWidget(m13Val);
   QDoubleSpinBox* m14Val = new QDoubleSpinBox(rightRow); rightRowLayout->addWidget(m14Val);

   QFrame* upRow = new QFrame(this); addWidget(upRow);
   QHBoxLayout* upRowLayout = new QHBoxLayout(upRow);
   upRowLayout->addWidget(new QLabel("up"));
   QDoubleSpinBox* m21Val = new QDoubleSpinBox(rightRow); upRowLayout->addWidget(m21Val);
   QDoubleSpinBox* m22Val = new QDoubleSpinBox(rightRow); upRowLayout->addWidget(m22Val);
   QDoubleSpinBox* m23Val = new QDoubleSpinBox(rightRow); upRowLayout->addWidget(m23Val);
   QDoubleSpinBox* m24Val = new QDoubleSpinBox(rightRow); upRowLayout->addWidget(m24Val);

   QFrame* lookRow = new QFrame(this); addWidget(lookRow);
   QHBoxLayout* lookRowLayout = new QHBoxLayout(lookRow);
   lookRowLayout->addWidget(new QLabel("look"));
   QDoubleSpinBox* m31Val = new QDoubleSpinBox(rightRow); lookRowLayout->addWidget(m31Val);
   QDoubleSpinBox* m32Val = new QDoubleSpinBox(rightRow); lookRowLayout->addWidget(m32Val);
   QDoubleSpinBox* m33Val = new QDoubleSpinBox(rightRow); lookRowLayout->addWidget(m33Val);
   QDoubleSpinBox* m34Val = new QDoubleSpinBox(rightRow); lookRowLayout->addWidget(m34Val);

   QFrame* posRow = new QFrame(this); addWidget(posRow);
   QHBoxLayout* posRowLayout = new QHBoxLayout(posRow);
   posRowLayout->addWidget(new QLabel("pos"));
   QDoubleSpinBox* m41Val = new QDoubleSpinBox(rightRow); posRowLayout->addWidget(m41Val);
   QDoubleSpinBox* m42Val = new QDoubleSpinBox(rightRow); posRowLayout->addWidget(m42Val);
   QDoubleSpinBox* m43Val = new QDoubleSpinBox(rightRow); posRowLayout->addWidget(m43Val);
   QDoubleSpinBox* m44Val = new QDoubleSpinBox(rightRow); posRowLayout->addWidget(m44Val);

   // set the start value
   D3DXMATRIX currVal = m_property.get();
   m11Val->setValue(currVal._11);
   m12Val->setValue(currVal._12);
   m13Val->setValue(currVal._13);
   m14Val->setValue(currVal._14);

   m21Val->setValue(currVal._21);
   m22Val->setValue(currVal._22);
   m23Val->setValue(currVal._23);
   m24Val->setValue(currVal._24);

   m31Val->setValue(currVal._31);
   m32Val->setValue(currVal._32);
   m33Val->setValue(currVal._33);
   m34Val->setValue(currVal._34);

   m41Val->setValue(currVal._41);
   m42Val->setValue(currVal._42);
   m43Val->setValue(currVal._43);
   m44Val->setValue(currVal._44);
   
   // connect the view to the model
   connect(m11Val, SIGNAL(valueChanged(double)), this, SLOT(m11ValChanged(double)));
   connect(m12Val, SIGNAL(valueChanged(double)), this, SLOT(m12ValChanged(double)));
   connect(m13Val, SIGNAL(valueChanged(double)), this, SLOT(m13ValChanged(double)));
   connect(m14Val, SIGNAL(valueChanged(double)), this, SLOT(m14ValChanged(double)));

   connect(m21Val, SIGNAL(valueChanged(double)), this, SLOT(m21ValChanged(double)));
   connect(m22Val, SIGNAL(valueChanged(double)), this, SLOT(m22ValChanged(double)));
   connect(m23Val, SIGNAL(valueChanged(double)), this, SLOT(m23ValChanged(double)));
   connect(m24Val, SIGNAL(valueChanged(double)), this, SLOT(m24ValChanged(double)));

   connect(m31Val, SIGNAL(valueChanged(double)), this, SLOT(m31ValChanged(double)));
   connect(m32Val, SIGNAL(valueChanged(double)), this, SLOT(m32ValChanged(double)));
   connect(m33Val, SIGNAL(valueChanged(double)), this, SLOT(m33ValChanged(double)));
   connect(m34Val, SIGNAL(valueChanged(double)), this, SLOT(m34ValChanged(double)));

   connect(m41Val, SIGNAL(valueChanged(double)), this, SLOT(m41ValChanged(double)));
   connect(m42Val, SIGNAL(valueChanged(double)), this, SLOT(m42ValChanged(double)));
   connect(m43Val, SIGNAL(valueChanged(double)), this, SLOT(m43ValChanged(double)));
   connect(m44Val, SIGNAL(valueChanged(double)), this, SLOT(m44ValChanged(double)));
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m11ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._11 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m12ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._12 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m13ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._13 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m14ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._14 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m21ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._21 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m22ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._22 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m23ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._23 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m24ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._24 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m31ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._31 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m32ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._32 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m33ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._33 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m34ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._34 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m41ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._41 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m42ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._42 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m43ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._43 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::m44ValChanged(double val)
{
   D3DXMATRIX newVal = m_property.get();
   newVal._44 = val;
   m_property.set(newVal);
}

///////////////////////////////////////////////////////////////////////////////
