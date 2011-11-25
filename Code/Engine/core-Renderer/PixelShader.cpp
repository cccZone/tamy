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
   PROPERTY( PixelShaderParams, m_params )
   PROPERTY( std::vector< TextureStageParams >, m_textureStages )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

PixelShader::PixelShader( const FilePath& resourceName )
   : Resource( resourceName )
   , m_entryFunctionName( "main" )
{
}

///////////////////////////////////////////////////////////////////////////////

PixelShader::~PixelShader()
{
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::onResourceLoaded( ResourcesManager& mgr )
{
   __super::onResourceLoaded( mgr );

   parseTextureStages();
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::setScript( const std::string& script ) 
{ 
   m_script = script;

   parseTextureStages();
   setDirty(); 
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< PixelShader >* PixelShader::createTextureSetter( const std::string& paramName, ShaderTexture& val )
{
   return val.createPixelShaderTextureSetter( paramName );
}

///////////////////////////////////////////////////////////////////////////////

void PixelShader::parseTextureStages()
{
   // parse constants
   ShaderCompiler compiler;
   m_textureStageName.clear();
   compiler.compilePixelShaderTextureStages( m_script, m_entryFunctionName.c_str(), m_textureStageName );

   // resize the number of supported texture
   unsigned int count = m_textureStageName.size();
   if ( count > m_textureStages.size() )
   {
      m_textureStages.resize( count );
   }
   else
   {
      // remove the unnecessary texture stages
      while ( count < (int)m_textureStages.size() )
      {
         m_textureStages.pop_back();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////