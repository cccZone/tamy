#include "RenderTargetDescriptorDialog.h"
#include "core-Renderer/RenderTargetDescriptor.h"


///////////////////////////////////////////////////////////////////////////////

RenderTargetDescriptorDialog::RenderTargetDescriptorDialog( QWidget* parent, RenderTargetDescriptor& descriptor, bool canChangeName )
   : QDialog( parent )
   , m_descriptor( descriptor )
{
   m_ui.setupUi( this );

   m_ui.isDynamicCheckBox->setChecked( false );
   updateTargetType();

   m_ui.renderTargetIdEdit->setEnabled( canChangeName );
   
   // fill the usages list
   {
      m_ui.usageComboBox->clear();
      TEnum< TextureUsage > textureUsagesEnum;
      std::vector< std::string > textureUsagesNames;
      textureUsagesEnum.getEnumerators( textureUsagesNames );

      unsigned int count = textureUsagesNames.size();
      for( unsigned int i = 0; i < count; ++i )
      {
         m_ui.usageComboBox->addItem( textureUsagesNames[i].c_str(), QVariant( i ) );
      }
   }

   // connect the widgets
   connect( m_ui.renderTargetIdEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( changeTargetID(const QString& ) ) );
   connect( m_ui.isReadableBox, SIGNAL( stateChanged( int ) ), this, SLOT( changeReadability( int ) ) );
   connect( m_ui.usageComboBox, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( usageChanged( const QString& ) ) );
   connect( m_ui.isDynamicCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( changeTargetType( int ) ) );
   connect( m_ui.widthScaleValueBox, SIGNAL( valueChanged( double ) ), this, SLOT( dynamicSizeChanged( double ) ) );
   connect( m_ui.heightScaleValueBox, SIGNAL( valueChanged( double ) ), this, SLOT( dynamicSizeChanged( double ) ) );
   connect( m_ui.widthValueBox, SIGNAL( valueChanged( int ) ), this, SLOT( staticSizeChanged( int ) ) );
   connect( m_ui.heightValueBox, SIGNAL( valueChanged( int ) ), this, SLOT( staticSizeChanged( int ) ) );

   // initialize the data fields
   m_ui.renderTargetIdEdit->setText( descriptor.getTargetID().c_str() );
   m_ui.isReadableBox->setChecked( descriptor.isReadable() );
   m_ui.isDynamicCheckBox->setChecked( descriptor.isDynamic() );

   unsigned int width, height;
   descriptor.getStaticSize( width, height );
   m_ui.widthValueBox->setMinimum(1);
   m_ui.widthValueBox->setMaximum(65535);
   m_ui.widthValueBox->setValue( width );
   m_ui.heightValueBox->setMinimum(1);
   m_ui.heightValueBox->setMaximum(65535);
   m_ui.heightValueBox->setValue( height );

   float widthScale, heightScale;
   descriptor.getDynamicSize( widthScale, heightScale );
   m_ui.widthScaleValueBox->setValue( widthScale );
   m_ui.heightScaleValueBox->setValue( heightScale );

   m_ui.usageComboBox->setCurrentIndex( (int)descriptor.getUsage() );
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
   TEnum< TextureUsage > textureUsagesEnum;
   TextureUsage usageVal = (TextureUsage)textureUsagesEnum.getValue( usage.toStdString() );
   
   m_descriptor.setUsage( usageVal );
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

void RenderTargetDescriptorDialog::staticSizeChanged( int val )
{
   int width = m_ui.widthValueBox->value();
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
