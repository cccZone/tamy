#include "core-Renderer\VertexShader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\ShaderParam.h"
#include "core-Renderer\ShaderTexture.h"
#include "core.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( VertexShader, Resource, tvsh, AM_BINARY )
   PROPERTY( std::string, m_script )
   PROPERTY( std::string, m_entryFunctionName )
   PROPERTY( VertexDescId, m_vertexDescId )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

VertexShader::VertexShader( const FilePath& resourceName )
   : Resource( resourceName )
   , m_entryFunctionName( "main" )
   , m_vertexDescId( VDI_SIMPLE )
{
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::setVertexDescription( VertexDescId vertexDescId )
{
   m_vertexDescId = vertexDescId;
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< VertexShader >* VertexShader::createTextureSetter( const std::string& paramName, ShaderTexture& val )
{
   return val.createVertexShaderTextureSetter( paramName );
}

///////////////////////////////////////////////////////////////////////////////
