#include "RenderTargetDescriptorDialog.h"
#include "core-Renderer/RenderTargetDescriptor.h"


///////////////////////////////////////////////////////////////////////////////

RenderTargetDescriptorDialog::RenderTargetDescriptorDialog( QWidget* parent, RenderTargetDescriptor& descriptor )
   : QDialog( parent )
   , m_descriptor( descriptor )
{
   m_ui.setupUi( this );

   m_ui.isDynamicCheckBox->setChecked( false );
   updateTargetType();

   // connect the widgets
   connect( m_ui.renderTargetIdEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( changeTargetID(const QString& ) ) );
   connect( m_ui.isReadableBox, SIGNAL( stateChanged( int ) ), this, SLOT( changeReadability( int ) ) );
   connect( m_ui.usageComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( usageChanged( const QString& ) ) );
   connect( m_ui.isDynamicCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( changeTargetType( int ) ) );
   connect( m_ui.widthScaleValueBox, SIGNAL( valueChanged( double ) ), this, SLOT( dynamicSizeChanged( double ) ) );
   connect( m_ui.heightScaleValueBox, SIGNAL( valueChanged( double ) ), this, SLOT( dynamicSizeChanged( double ) ) );
   connect( m_ui.widthValueBox, SIGNAL( valueChanged( int ) ), this, SLOT( staticcSizeChanged( int ) ) );
   connect( m_ui.heightValueBox, SIGNAL( valueChanged( int ) ), this, SLOT( staticcSizeChanged( int ) ) );
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptorDialog::changeTargetID( const QString& value )
{
   m_descriptor.setTargetID( value.toStdString() );
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptorDialog::changeReadability( int isReadable )
{
   m_descriptor.setReadable( isReadable != 0 );
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptorDialog::usageChanged( const QString& usage )
{
   if ( usage == "Color" )
   {
      m_descriptor.setUsage( TU_COLOR );
   }
   else if ( usage == "Depth" )
   {
      m_descriptor.setUsage( TU_DEPTH );
   }
   else if ( usage == "Luminance" )
   {
      m_descriptor.setUsage( TU_LUMINANCE );
   }
   else if ( usage == "HDR" )
   {
      m_descriptor.setUsage( TU_HDR );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptorDialog::changeTargetType( int isDynamic )
{
   m_descriptor.setType( isDynamic != 0 );
   updateTargetType();
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptorDialog::dynamicSizeChanged( double val )
{
   float widthScale = m_ui.widthScaleValueBox->value();
   float heightScale = m_ui.heightScaleValueBox->value();
   m_descriptor.setDynamicSize( widthScale, heightScale );
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptorDialog::staticSizeChanged( double val )
{
   float width = m_ui.widthValueBox->value();
   float height = m_ui.heightValueBox->value();
   m_descriptor.setStaticSize( width, height );
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetDescriptorDialog::updateTargetType()
{
   bool isDynamic = m_ui.isDynamicCheckBox->isChecked();
   m_ui.widthScaleValueBox->setEnabled( isDynamic );
   m_ui.heightScaleValueBox->setEnabled( isDynamic );
   m_ui.widthValueBox->setEnabled( !isDynamic );
   m_ui.heightValueBox->setEnabled( !isDynamic );
}

///////////////////////////////////////////////////////////////////////////////