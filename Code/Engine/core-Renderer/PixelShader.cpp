#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\ShaderParam.h"
#include "core-Renderer\ShaderTexture.h"
#include "core-Renderer\PixelShaderConstant.h"
#include "core-Renderer\ShaderCompiler.h"
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
   : Resource( fileName )
   , m_entryFunctionName( "main" )
{
}

///////////////////////////////////////////////////////////////////////////////

PixelShader::~PixelShader()
{
   unsigned int count = m_constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_constants[i];
   }
   m_constants.clear();
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
   StreamBuffer<char> shaderScript( *file );
   m_script = shaderScript.getBuffer();
   delete file;

   m_entryFunctionName = entryFunctionName;
   
   // parse it
   parseShaderConstants();
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::onResourceLoaded( ResourcesManager& mgr )
{
   __super::onResourceLoaded( mgr );

   parseShaderConstants();
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::setScript( const std::string& script ) 
{ 
   m_script = script;

   parseShaderConstants();
   setDirty(); 
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< PixelShader >* PixelShader::createTextureSetter( const std::string& paramName, ShaderTexture& val )
{
   return val.createPixelShaderTextureSetter( paramName );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::parseShaderConstants()
{
   // delete old constants
   unsigned int count = m_constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_constants[i];
   }
   m_constants.clear();

   // parse constants
   ShaderCompiler compiler;
   compiler.compilePixelShader( m_script, m_entryFunctionName.c_str(), &m_constants );
}

///////////////////////////////////////////////////////////////////////////////
