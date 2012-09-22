#include "core-Renderer/GNVertexShader.h"
#include "core-Renderer/VertexShader.h"
#include "core-Renderer/GeometryEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GNVertexShader );
   PARENT( GeometryShaderNode );
   PROPERTY_EDIT( "Shader", VertexShader*, m_shader );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GNVertexShader::GNVertexShader()
   : m_shader( NULL )
   , m_shaderNode( NULL )
{
   m_shaderNode = new VertexShaderNodeOperator< GeometryShaderNode >( *this );
}

///////////////////////////////////////////////////////////////////////////////

GNVertexShader::~GNVertexShader()
{
   delete m_shaderNode; m_shaderNode = NULL;
   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GNVertexShader::onPrePropertyChanged( ReflectionProperty& property )
{
   __super::onPrePropertyChanged( property );

   if ( property.getName() == "m_shader" && m_shaderNode )
   {
      m_shaderNode->resetShader();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GNVertexShader::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_shader" && m_shader )
   {
      m_shaderNode->setShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void GNVertexShader::onGraphLoaded()
{
   if ( m_shader )
   {
      m_shaderNode->setShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

RCBindVertexShader* GNVertexShader::createRenderCommand( Renderer& renderer, const GeometryEntity& entity ) const
{
   RCBindVertexShader* comm = NULL;
   if ( m_shader && m_shaderNode )
   {
      comm = m_shaderNode->bindShader( renderer, entity.data() );
   }

   return comm;
}

///////////////////////////////////////////////////////////////////////////////

void GNVertexShader::preRender( Renderer& renderer, const GeometryEntity& entity ) const
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void GNVertexShader::postRender( Renderer& renderer, const GeometryEntity& entity ) const
{
   if ( m_shader )
   {
      new ( renderer() ) RCUnbindVertexShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////
