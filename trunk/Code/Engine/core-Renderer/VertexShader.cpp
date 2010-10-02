#include "core-Renderer\VertexShader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\LitVertex.h"
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

void VertexShader::onComponentAdded( Component< ResourcesManager >& component )
{
   ResourceManagerComponent< Renderer >* rendererComp = dynamic_cast< ResourceManagerComponent< Renderer >* >( &component );
   if ( rendererComp )
   {
      rendererComp->get().implement< VertexShader >( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::onComponentRemoved( Component< ResourcesManager >& component )
{
   ResourceManagerComponent< Renderer >* rendererComp = dynamic_cast< ResourceManagerComponent< Renderer >* >( &component );
   if ( rendererComp )
   {
      setImplementation( NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::beginRendering()
{
   impl().beginRendering();
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::endRendering()
{
   impl().endRendering();
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::setBool( const char* paramName, bool val )
{
   impl().setBool( paramName, val );
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::setMtx( const char* paramName, const D3DXMATRIX& matrix )
{
   impl().setMtx( paramName, matrix );
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int count )
{
   impl().setMtxArray( paramName, matrices, count );
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::setVec4( const char* paramName, const D3DXVECTOR4& vec )
{
   impl().setVec4( paramName, vec );
}

///////////////////////////////////////////////////////////////////////////////

void VertexShader::setTexture( const char* paramName, ShaderTexture& val )
{
   impl().setTexture( paramName, val );
}

///////////////////////////////////////////////////////////////////////////////
