/// @file   dx9-Renderer/DX9PixelShaderConstantsCompiler.h
/// @brief  pixel shader constants compiler tool
#ifndef _DX9_PIXEL_SHADER_CONSTANTS_COMPILER_H
#define _DX9_PIXEL_SHADER_CONSTANTS_COMPILER_H

#include "core-Renderer/PixelShaderConstant.h"
#include <string>
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
class DX9PixelShaderConstantsCompiler
{
public:
   std::string m_errorMsg;

public:
   bool compile( const std::string& shaderCode, const char* entryFunction, std::vector< typename PixelShaderConstant< TNode >* >& outConstants );

private:
   PixelShaderConstant< TNode >* createScalarConstant( const D3DXCONSTANT_DESC& desc ) const;
   PixelShaderConstant< TNode >* createObjectConstant( const D3DXCONSTANT_DESC& desc ) const;
   PixelShaderConstant< TNode >* createMatrixConstant( const D3DXCONSTANT_DESC& desc ) const;
   PixelShaderConstant< TNode >* createConstant( const D3DXCONSTANT_DESC& desc ) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "dx9-Renderer/DX9PixelShaderConstantsCompiler.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _DX9_PIXEL_SHADER_CONSTANTS_COMPILER_H
