#include "core-Renderer\Shader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\LitVertex.h"
#include "core.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( Shader, Resource, tsh, AM_BINARY )
   PROPERTY( ShaderType, m_type )
   PROPERTY( VertexDescId, m_vertexDescId )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

Shader::Shader()
   : m_type( SHT_VERTEX_SHADER )
   , m_vertexDescId( VDI_SIMPLE )
{
}

///////////////////////////////////////////////////////////////////////////////

Shader::Shader( const std::string& scriptPath, ShaderType type )
   : Resource( scriptPath )
   , m_type( type )
   , m_vertexDescId( VDI_SIMPLE ) 
{
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setVertexDescription( VertexDescId vertexDescId )
{
   ASSERT( m_type == SHT_VERTEX_SHADER, "Only vertex shaders support vertex descriptions" );
   m_vertexDescId = vertexDescId;

}

///////////////////////////////////////////////////////////////////////////////

void Shader::onResourceLoaded( ResourcesManager& mgr )
{
   std::string scriptPath = getFilePath();
   if ( m_script.empty() )
   {
      ASSERT( !scriptPath.empty(), "Neither .vsh file nor a shader script specified" );
      if ( scriptPath.empty() )
      {
         return;
      }

      const Filesystem& fs = mgr.getFilesystem();
      File* file = fs.open( scriptPath, std::ios_base::in | std::ios_base::binary );
      if (file == NULL)
      {
         throw std::invalid_argument("HLSL vertex shader file doesn't exist");
      }
      StreamBuffer<byte> buf(*file);

      StreamBuffer<char> shaderScript( *file );
      m_script = shaderScript.getBuffer();
      delete file;
   }
}

///////////////////////////////////////////////////////////////////////////////

void Shader::onComponentAdded( Component< ResourcesManager >& component )
{
   ResourceManagerComponent< Renderer >* rendererComp = dynamic_cast< ResourceManagerComponent< Renderer >* >( &component );
   if ( rendererComp )
   {
      rendererComp->get().implement< Shader >( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Shader::onComponentRemoved( Component< ResourcesManager >& component )
{
   ResourceManagerComponent< Renderer >* rendererComp = dynamic_cast< ResourceManagerComponent< Renderer >* >( &component );
   if ( rendererComp )
   {
      setImplementation( NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Shader::beginRendering()
{
   impl().beginRendering();
}

///////////////////////////////////////////////////////////////////////////////

void Shader::endRendering()
{
   impl().endRendering();
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setBool( const char* paramName, bool val )
{
   impl().setBool( paramName, val );
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setMtx( const char* paramName, const D3DXMATRIX& matrix )
{
   impl().setMtx( paramName, matrix );
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int count )
{
   impl().setMtxArray( paramName, matrices, count );
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setVec4( const char* paramName, const D3DXVECTOR4& vec )
{
   impl().setVec4( paramName, vec );
}

///////////////////////////////////////////////////////////////////////////////

void Shader::setTexture( const char* paramName, ShaderTexture& val )
{
   impl().setTexture( paramName, val );
}

///////////////////////////////////////////////////////////////////////////////
