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
   unsigned int count = m_constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_constants[i];
   }
   m_constants.clear();
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

   // resize the number of supported texture
   int maxTexStageIdx = -1;
   count = m_constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      int idx = m_constants[i]->getRequiredTextureStageIdx();
      if ( idx < 0 )
      {
         continue;
      }

      if ( maxTexStageIdx < idx )
      {
         maxTexStageIdx = idx;
      }
      if ( idx >= (int)m_textureStages.size() )
      {
         m_textureStages.resize( idx + 1 );
      }
      if ( idx >= (int)m_textureStageName.size() )
      {
         m_textureStageName.resize( idx + 1 );
      }

      m_textureStageName[idx] = m_constants[i]->getName();
   }

   // remove the unnecessary texture stages
   ++maxTexStageIdx;
   while ( maxTexStageIdx < (int)m_textureStages.size() )
   {
      m_textureStages.pop_back();
   }
}

///////////////////////////////////////////////////////////////////////////////