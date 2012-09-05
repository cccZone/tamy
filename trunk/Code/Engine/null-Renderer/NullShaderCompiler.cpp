#include "core-Renderer/ShaderCompiler.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/MaterialNode.h"
#include "core-Renderer/GeometryShaderNode.h"


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

bool ShaderCompiler::compilePixelShaderConstants( const std::string& shaderCode, const char* entryFunction, std::vector< ShaderConstantDesc >& outConstants )
{
   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool ShaderCompiler::compileVertexShader( const std::string& shaderCode, const char* entryFunction )
{
   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool ShaderCompiler::compileVertexShaderConstants( const std::string& shaderCode, const char* entryFunction, std::vector< ShaderConstantDesc >& outConstants )
{
   return true;
}

///////////////////////////////////////////////////////////////////////////////
