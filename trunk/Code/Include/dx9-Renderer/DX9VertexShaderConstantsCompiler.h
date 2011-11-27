/// @file   dx9-Renderer/DX9VertexShaderConstantsCompiler.h
/// @brief  vertex shader constants compiler tool
#ifndef _DX9_VERTEX_SHADER_CONSTANTS_COMPILER_H
#define _DX9_VERTEX_SHADER_CONSTANTS_COMPILER_H

#include "core-Renderer/VertexShaderConstant.h"
#include <string>
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
class DX9VertexShaderConstantsCompiler
{
public:
   std::string m_errorMsg;

public:
   bool compile( const std::string& shaderCode, const char* entryFunction, std::vector< typename VertexShaderConstant< TNode >* >& outConstants );

private:
   VertexShaderConstant< TNode >* createScalarConstant( const D3DXCONSTANT_DESC& desc ) const;
   VertexShaderConstant< TNode >* createObjectConstant( const D3DXCONSTANT_DESC& desc ) const;
   VertexShaderConstant< TNode >* createMatrixConstant( const D3DXCONSTANT_DESC& desc ) const;
   VertexShaderConstant< TNode >* createConstant( const D3DXCONSTANT_DESC& desc ) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "dx9-Renderer/DX9VertexShaderConstantsCompiler.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _DX9_VERTEX_SHADER_CONSTANTS_COMPILER_H
