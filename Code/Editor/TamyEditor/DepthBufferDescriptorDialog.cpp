#include "DepthBufferDescriptorDialog.h"
#include "core-Renderer/DepthBufferDescriptor.h"


///////////////////////////////////////////////////////////////////////////////

DepthBufferDescriptorDialog::DepthBufferDescriptorDialog( QWidget* parent, DepthBufferDescriptor& descriptor, bool canChangeName )
   : QDialog( parent )
   , m_descriptor( descriptor )
{
   m_ui.setupUi( this );

   m_ui.depthBufferIdEdit->setEnabled( canChangeName );

   // connect the widgets
   connect( m_ui.depthBufferIdEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( changeBufferID(const QString& ) ) );
   connect( m_ui.widthValueBox, SIGNAL( valueChanged( int ) ), this, SLOT( sizeChanged( int ) ) );
   connect( m_ui.heightValueBox, SIGNAL( valueChanged( int ) ), this, SLOT( sizeChanged( int ) ) );

   // initialize the data fields
   m_ui.depthBufferIdEdit->setText( descriptor.getID().c_str() );

   unsigned int width, height;
   descriptor.getSize( width, height );
   m_ui.widthValueBox->setMinimum(1);
   m_ui.widthValueBox->setMaximum(65535);
   m_ui.widthValueBox->setValue( width );
   m_ui.heightValueBox->setMinimum(1);
   m_ui.heightValueBox->setMaximum(65535);
   m_ui.heightValueBox->setValue( height );
}

///////////////////////////////////////////////////////////////////////////////

void DepthBufferDescriptorDialog::changeBufferID( const QString& value )
{
   m_descriptor.setID( value.toStdString() );
}

///////////////////////////////////////////////////////////////////////////////

void DepthBufferDescriptorDialog::sizeChanged( int val )
{
   int width = m_ui.widthValueBox->value();
   float height = m_ui.heightValueBox->value();
   m_descriptor.setSize( width, height );
}

///////////////////////////////////////////////////////////////////////////////
