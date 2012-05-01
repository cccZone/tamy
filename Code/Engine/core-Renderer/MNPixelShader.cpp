#include "core-Renderer/MNPixelShader.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNPixelShader );
   PARENT( MaterialNode );
   PROPERTY_EDIT( "Shader", PixelShader*, m_shader );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MNPixelShader::MNPixelShader()
   : m_shader( NULL )
   , m_shaderNode( NULL )
{
   m_shaderNode = new PixelShaderNodeOperator< MaterialNode >( *this );
}

///////////////////////////////////////////////////////////////////////////////

MNPixelShader::~MNPixelShader()
{
   delete m_shaderNode; m_shaderNode = NULL;
   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNPixelShader::onObjectLoaded()
{
   __super::onObjectLoaded();

   if ( m_shader )
   {
      m_shaderNode->setShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MNPixelShader::onPrePropertyChanged( ReflectionProperty& property )
{
   __super::onPrePropertyChanged( property );

   if ( property.getName() == "m_shader" && m_shaderNode )
   {
      m_shaderNode->resetShader();
   }
}

///////////////////////////////////////////////////////////////////////////////

void MNPixelShader::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_shader" && m_shader )
   {
      m_shaderNode->setShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MNPixelShader::preRender( Renderer& renderer, const MaterialEntity& entity ) const
{
   if ( !m_shader || !m_shaderNode )
   {
      return;
   }

   // bind the shader
   m_shaderNode->bindShader( renderer, entity.data() );
}

///////////////////////////////////////////////////////////////////////////////

void MNPixelShader::postRender( Renderer& renderer, const MaterialEntity& entity ) const
{
   if ( m_shader )
   {
      new ( renderer() ) RCUnbindPixelShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////
