#include "core-Renderer/MNPixelShader.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/MaterialInstance.h"


// TODO: !!!!!!!!! (BUGS) !!!!!!!!!!!!!!!!!
// - random crash on exit

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

void MNPixelShader::onGraphLoaded()
{
   if ( m_shader )
   {
      // set the shader on the object
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

void MNPixelShader::preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   if ( !m_shader || !m_shaderNode )
   {
      return;
   }

   // bind the shader
   m_shaderNode->bindShader( renderer, instance.data() );
}

///////////////////////////////////////////////////////////////////////////////

void MNPixelShader::postRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   if ( m_shader )
   {
      new ( renderer() ) RCUnbindPixelShader( *m_shader, renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////
