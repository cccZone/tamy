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
   PROPERTY( VertexDescId, m_vertexDescId )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

VertexShader::VertexShader()
   : m_vertexDescId( VDI_SIMPLE )
{
}

///////////////////////////////////////////////////////////////////////////////

VertexShader::VertexShader( const std::string& scriptPath )
   : Resource( scriptPath )
   , m_vertexDescId( VDI_SIMPLE ) 
{
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::setVertexDescription( VertexDescId vertexDescId )
{
   m_vertexDescId = vertexDescId;
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::onResourceLoaded( ResourcesManager& mgr )
{
   std::string scriptPath = getFilePath();
   if ( m_script.empty() )
   {
      ASSERT_MSG( !scriptPath.empty(), "Neither .vsh file nor a shader script specified" );
      if ( scriptPath.empty() )
      {
         return;
      }

      const Filesystem& fs = mgr.getFilesystem();
      File* file = fs.open( scriptPath, std::ios_base::in | std::ios_base::binary );
      if (file == NULL)
      {
         std::string errorMsg = std::string( "HLSL vertex shader file " ) + scriptPath + " doesn't exist";
         ASSERT_MSG( false, errorMsg.c_str() );
         return;
      }
      StreamBuffer<byte> buf(*file);

      StreamBuffer<char> shaderScript( *file );
      m_script = shaderScript.getBuffer();
      delete file;
   }
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< VertexShader >* VertexShader::createTextureSetter( const std::string& paramName, ShaderTexture& val )
{
   return val.createVertexShaderTextureSetter( paramName );
}

///////////////////////////////////////////////////////////////////////////////
