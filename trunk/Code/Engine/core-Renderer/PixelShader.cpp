#include "core-Renderer\PixelShader.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\ShaderParam.h"
#include "core-Renderer\ShaderTexture.h"
#include "core-Renderer\PixelShaderConstant.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\ShaderCompiler.h"
#include "core.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

OutStream& operator<<( OutStream& stream, const TextureStageParams& params )
{
   stream << params.m_addressU;
   stream << params.m_addressV;
   stream << params.m_addressW;
   stream << params.m_minFilter;
   stream << params.m_magFilter;
   stream << params.m_mipFilter;
   return stream;
}

///////////////////////////////////////////////////////////////////////////////

InStream& operator>>( InStream& stream, TextureStageParams& params )
{
   stream >> params.m_addressU;
   stream >> params.m_addressV;
   stream >> params.m_addressW;
   stream >> params.m_minFilter;
   stream >> params.m_magFilter;
   stream >> params.m_mipFilter;
   return stream;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

OutStream& operator<<( OutStream& stream, const PixelShaderParams& params )
{
   stream << params.m_cullingMode;
   stream << params.m_useZBuffer;
   stream << params.m_writeToZBuffer;
   stream << params.m_depthTestFunc;
   stream << params.m_useAlphaTest;
   stream << params.m_alphaTestFunc;
   stream << params.m_alphaTestRefVal;
   stream << params.m_useBlending;
   stream << params.m_blendSourceFunc;
   stream << params.m_blendDestFunc;
   stream << params.m_useSeparateAlphaBlend;
   stream << params.m_alphaBlendSourceFunc;
   stream << params.m_alphaBlendDestFunc;   
   stream << params.m_stencilEnable;
   stream << params.m_stencilFail;
   stream << params.m_stencilZFail;
   stream << params.m_stencilPass;
   stream << params.m_stencilFunc;
   stream << params.m_stencilRef;
   stream << params.m_stencilMask;
   stream << params.m_stencilWriteMask;

   return stream;
}

///////////////////////////////////////////////////////////////////////////////

InStream& operator>>( InStream& stream, PixelShaderParams& params )
{
   stream >> params.m_cullingMode;
   stream >> params.m_useZBuffer;
   stream >> params.m_writeToZBuffer;
   stream >> params.m_depthTestFunc;
   stream >> params.m_useAlphaTest;
   stream >> params.m_alphaTestFunc;
   stream >> params.m_alphaTestRefVal;
   stream >> params.m_useBlending;
   stream >> params.m_blendSourceFunc;
   stream >> params.m_blendDestFunc;
   stream >> params.m_useSeparateAlphaBlend;
   stream >> params.m_alphaBlendSourceFunc;
   stream >> params.m_alphaBlendDestFunc;
   stream >> params.m_stencilEnable;
   stream >> params.m_stencilFail;
   stream >> params.m_stencilZFail;
   stream >> params.m_stencilPass;
   stream >> params.m_stencilFunc;
   stream >> params.m_stencilRef;
   stream >> params.m_stencilMask;
   stream >> params.m_stencilWriteMask;

   return stream;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( PixelShader, tpsh, AM_BINARY )
   PROPERTY( std::string, m_script )
   PROPERTY( std::string, m_entryFunctionName )
   PROPERTY( PixelShaderParams, m_params )
   PROPERTY( std::vector< TextureStageParams >, m_textureStages )
   PROPERTY( uint, m_requiredVertexShaderTechniqueId )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

PixelShader::PixelShader( const FilePath& resourceName )
   : Resource( resourceName )
   , m_entryFunctionName( "main" )
   , m_requiredVertexShaderTechniqueId( 0 )
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

   ShaderCompiler compiler;
   std::string techniqueName = compiler.parseRequiredVertexShaderTechnique( m_script );
   m_requiredVertexShaderTechniqueId = VertexShader::generateTechniqueId( techniqueName );

   setDirty(); 
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< PixelShader >* PixelShader::createTextureSetter( MemoryPoolAllocator& allocator, const IDString& paramName, ShaderTexture& val )
{
   return val.createPixelShaderTextureSetter( allocator, paramName );
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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RCBindPixelShader::RCBindPixelShader( PixelShader& shader, Renderer& renderer )
   : ShaderRenderCommand< PixelShader >( *renderer.getAllocator() )
   , m_shader( shader )
{
   uint techniqueId = m_shader.getRequiredVertexShaderTechnique();
   renderer.setVertexShaderTechnique( techniqueId );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RCUnbindPixelShader::RCUnbindPixelShader( PixelShader& shader, Renderer& renderer )
   : m_shader( shader )
{
   renderer.resetVertexShaderTechnique();
}

///////////////////////////////////////////////////////////////////////////////
