/// @file   dx9-Renderer/DX9PixelShaderConstantsCompiler.h
/// @brief  pixel shader constants compiler tool
#ifndef _DX9_PIXEL_SHADER_CONSTANTS_COMPILER_H
#define _DX9_PIXEL_SHADER_CONSTANTS_COMPILER_H

#include "core-Renderer/PixelShaderConstant.h"
#include <string>
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9PixelShaderConstantsCompiler
{
public:
   std::string m_errorMsg;

public:
   bool compile( const std::string& shaderCode, const char* entryFunction, std::vector< ShaderConstantDesc >& outConstants );

private:
   bool createScalarConstant( const D3DXCONSTANT_DESC& desc, ShaderConstantDesc& outDesc ) const;
   bool createObjectConstant( const D3DXCONSTANT_DESC& desc, ShaderConstantDesc& outDesc ) const;
   bool createMatrixConstant( const D3DXCONSTANT_DESC& desc, ShaderConstantDesc& outDesc ) const;
   bool createConstant( const D3DXCONSTANT_DESC& desc, ShaderConstantDesc& outDesc ) const;
};

///////////////////////////////////////////////////////////////////////////////

#endif // _DX9_PIXEL_SHADER_CONSTANTS_COMPILER_H
