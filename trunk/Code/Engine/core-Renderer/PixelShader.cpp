#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\LitVertex.h"
#include "core.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( PixelShader, Resource, tpsh, AM_BINARY )
   PROPERTY( std::string, m_script )
   PROPERTY( std::string, m_entryFunctionName )
   PROPERTY(PixelShaderParams, m_params )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

PixelShader::PixelShader( const std::string& fileName )
   : Resource( Filesystem::changeFileExtension( fileName, PixelShader::getExtension() ) )
   , m_entryFunctionName( "main" )
{
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::loadFromFile( const Filesystem& fs, const std::string& fileName, const std::string& entryFunctionName )
{
   ASSERT_MSG( !fileName.empty(), "Neither .psh file nor a shader script specified" );
   if ( fileName.empty() )
   {
      return;
   }

   File* file = fs.open( fileName, std::ios_base::in | std::ios_base::binary );
   if (file == NULL)
   {
      throw std::invalid_argument("HLSL vertex shader file doesn't exist");
   }
   StreamBuffer<byte> buf(*file);

   StreamBuffer<char> shaderScript( *file );
   m_script = shaderScript.getBuffer();
   delete file;

   m_entryFunctionName = entryFunctionName;
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::onComponentAdded( Component< ResourcesManager >& component )
{
   ResourceManagerComponent< Renderer >* rendererComp = dynamic_cast< ResourceManagerComponent< Renderer >* >( &component );
   if ( rendererComp )
   {
      rendererComp->get().implement< PixelShader >( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::onComponentRemoved( Component< ResourcesManager >& component )
{
   ResourceManagerComponent< Renderer >* rendererComp = dynamic_cast< ResourceManagerComponent< Renderer >* >( &component );
   if ( rendererComp )
   {
      setImplementation( NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::beginRendering()
{
   impl().beginRendering();
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::endRendering()
{
   impl().endRendering();
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::setBool( const char* paramName, bool val )
{
   impl().setBool( paramName, val );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::setFloat( const char* paramName, float val )
{
   impl().setFloat( paramName, val );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::setFloatArray( const char* paramName, float* valsArr, unsigned int size )
{
   impl().setFloatArray( paramName, valsArr, size );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::setMtx( const char* paramName, const D3DXMATRIX& matrix )
{
   impl().setMtx( paramName, matrix );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int size )
{
   impl().setMtxArray( paramName, matrices, size );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::setVec4( const char* paramName, const D3DXVECTOR4& vec )
{
   impl().setVec4( paramName, vec );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::setVec4Array( const char* paramName, const D3DXVECTOR4* vecArr, unsigned int size )
{
   impl().setVec4Array( paramName, vecArr, size );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::setTexture( const char* paramName, ShaderTexture& val )
{
   impl().setTexture( paramName, val );
}

///////////////////////////////////////////////////////////////////////////////
