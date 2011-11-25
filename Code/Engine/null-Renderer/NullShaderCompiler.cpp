#include "core-Renderer/ShaderCompiler.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

bool ShaderCompiler::compilePixelShader( const std::string& shaderCode, const char* entryFunction )
{
   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool ShaderCompiler::compilePixelShaderTextureStages( const std::string& shaderCode, const char* entryFunction, std::vector< std::string >& outNames )
{
   return true;
}

///////////////////////////////////////////////////////////////////////////////

template<>
bool ShaderCompiler::compilePixelShaderConstants( const std::string& shaderCode, const char* entryFunction, std::vector< typename PixelShaderConstant< RenderingPipelineNode >* >& outConstants )
{
   return true;
}

///////////////////////////////////////////////////////////////////////////////

template<>
bool ShaderCompiler::compilePixelShaderConstants( const std::string& shaderCode, const char* entryFunction, std::vector< typename PixelShaderConstant< MaterialNode >* >& outConstants )
{
   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool ShaderCompiler::compileVertexShader( const std::string& shaderCode, const char* entryFunction )
{
   return true;
}

///////////////////////////////////////////////////////////////////////////////
