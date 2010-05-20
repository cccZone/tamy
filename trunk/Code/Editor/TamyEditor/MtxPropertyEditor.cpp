#include "MtxPropertyEditor.h"
#include <QGroupBox.h>
#include <QBoxLayout.h>
#include <QLabel.h>
#include <QSpinBox.h>
#include "core\Vector.h"
#include "core\EulerAngles.h"
#include "core\Matrix.h"


///////////////////////////////////////////////////////////////////////////////

MtxPropertyEditor::MtxPropertyEditor( TEditableProperty< D3DXMATRIX >* property )
: QPropertyEditor( property->getLabel().c_str() )
, m_property( property )
{
   setupUi();
}

///////////////////////////////////////////////////////////////////////////////

MtxPropertyEditor::~MtxPropertyEditor()
{
   delete m_property;
   m_property = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::setupUi()
{
   QGroupBox* rotRow = new QGroupBox( "orientation", this ); addWidget(rotRow);
   QHBoxLayout* rotRowLayout = new QHBoxLayout(rotRow);
   rotRowLayout->setSpacing( 0 );
   rotRowLayout->setMargin( 0 );

   QDoubleSpinBox* yawVal = new QDoubleSpinBox(rotRow); 
   rotRowLayout->addWidget(new QLabel("yaw:", rotRow)); 
   rotRowLayout->addWidget(yawVal);

   QDoubleSpinBox* pitchVal = new QDoubleSpinBox(rotRow); 
   rotRowLayout->addWidget(new QLabel("pitch:", rotRow));
   rotRowLayout->addWidget(pitchVal);

   QDoubleSpinBox* rollVal = new QDoubleSpinBox(rotRow); 
   rotRowLayout->addWidget(new QLabel("roll:", rotRow));
   rotRowLayout->addWidget(rollVal);

   QGroupBox* posRow = new QGroupBox( "translation", this ); addWidget(posRow);
   QHBoxLayout* posRowLayout = new QHBoxLayout(posRow);
   posRowLayout->setSpacing( 0 );
   posRowLayout->setMargin( 0 );
   QDoubleSpinBox* xVal = new QDoubleSpinBox(posRow);
   posRowLayout->addWidget(new QLabel("x:", posRow));
   posRowLayout->addWidget(xVal);
   QDoubleSpinBox* yVal = new QDoubleSpinBox(posRow); 
   posRowLayout->addWidget(new QLabel("y:", posRow));
   posRowLayout->addWidget(yVal);
   QDoubleSpinBox* zVal = new QDoubleSpinBox(posRow); 
   posRowLayout->addWidget(new QLabel("z:", posRow));
   posRowLayout->addWidget(zVal);

   // set the start value
   Matrix currVal = m_property->get();

   EulerAngles orientation = currVal;
   yawVal->setValue(orientation.yaw);
   pitchVal->setValue(orientation.pitch);
   rollVal->setValue(orientation.roll);

   Vector pos = currVal.position();
   xVal->setValue(pos.x);
   yVal->setValue(pos.y);
   zVal->setValue(pos.z);
   
   // connect the view to the model
   connect(yawVal, SIGNAL(valueChanged(double)), this, SLOT(yawValChanged(double)));
   connect(pitchVal, SIGNAL(valueChanged(double)), this, SLOT(pitchValChanged(double)));
   connect(rollVal, SIGNAL(valueChanged(double)), this, SLOT(rollValChanged(double)));

   connect(xVal, SIGNAL(valueChanged(double)), this, SLOT(xValChanged(double)));
   connect(yVal, SIGNAL(valueChanged(double)), this, SLOT(yValChanged(double)));
   connect(zVal, SIGNAL(valueChanged(double)), this, SLOT(zValChanged(double)));
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::yawValChanged(double val)
{
   Matrix currVal = m_property->get();

   EulerAngles orientation = currVal;
   orientation.yaw = (float)val;
   
   Matrix rotMtx( orientation );
   Matrix transMtx( currVal.position() );

   m_property->set( transMtx * rotMtx );
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::pitchValChanged(double val)
{
   Matrix currVal = m_property->get();

   EulerAngles orientation = currVal;
   orientation.pitch = (float)val;

   Matrix rotMtx( orientation );
   Matrix transMtx( currVal.position() );

   m_property->set( transMtx * rotMtx );
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::rollValChanged(double val)
{
   Matrix currVal = m_property->get();

   EulerAngles orientation = currVal;
   orientation.roll = (float)val;

   Matrix rotMtx( orientation );
   Matrix transMtx( currVal.position() );

   m_property->set( transMtx * rotMtx );
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::xValChanged(double val)
{
   Matrix currVal = m_property->get();

   Vector pos = currVal.position();
   pos.x = (float)val;

   Matrix rotMtx( (EulerAngles)currVal );
   Matrix transMtx( pos );

   m_property->set( transMtx * rotMtx );
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::yValChanged(double val)
{
   Matrix currVal = m_property->get();

   Vector pos = currVal.position();
   pos.y = (float)val;

   Matrix rotMtx( (EulerAngles)currVal );
   Matrix transMtx( pos );

   m_property->set( transMtx * rotMtx );
}

///////////////////////////////////////////////////////////////////////////////

void MtxPropertyEditor::zValChanged(double val)
{
   Matrix currVal = m_property->get();

   Vector pos = currVal.position();
   pos.z = (float)val;

   Matrix rotMtx( (EulerAngles)currVal );
   Matrix transMtx( pos );

   m_property->set( transMtx * rotMtx );
}

///////////////////////////////////////////////////////////////////////////////
