#include "core-Renderer/MNPixelShader.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/MaterialEntity.h"


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

void MNPixelShader::onObjectLoaded()
{
   __super::onObjectLoaded();

   if ( m_shader )
   {
      // set the shader on the object
      m_shaderNode->setShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MNPixelShader::onGraphLoaded()
{
   // remove the non-existing sockets
   std::vector< std::string > sockets;
   const std::vector< TInputSocket* >& inputs = getInputs();
   uint count = inputs.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( inputs[i] != NULL )
      {
         sockets.push_back( inputs[i]->getName() );
      }
   }

   m_shaderNode->filterSockets( sockets );

   // remove non-existing sockets
   count = sockets.size();
   for ( uint i = 0; i < count; ++i )
   {
      removeInput( sockets[i] );
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
      new ( renderer() ) RCUnbindPixelShader( *m_shader, renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////
