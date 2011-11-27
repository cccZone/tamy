#ifndef _DX9_PIXEL_SHADER_CONSTANTS_COMPILER_H
#error "This file can only be included from DX9PixelShaderConstantsCompiler.h"
#else

#include "core-Renderer/PixelShaderConstant.h"
#include "dx9-Renderer/DX9ShaderIncludeLoader.h"
#include "dx9-Renderer/DXErrorParser.h"
#include "dx9-Renderer/DX9Initializer.h"


///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PixelShaderConstant< TNode >* DX9PixelShaderConstantsCompiler< TNode >::createScalarConstant( const D3DXCONSTANT_DESC& desc ) const
{
   switch( desc.Type )
   {
   case D3DXPT_BOOL:
      {
         return new PSCBool< TNode >( desc.Name, false );
      }

   case D3DXPT_INT:
      {
         return new PSCInt< TNode >( desc.Name, 0 );
      }

   case D3DXPT_FLOAT:
      {
         return new PSCFloat< TNode >( desc.Name, 0.0f );
      }

   case D3DXPT_STRING:
      {
         return new PSCString< TNode >( desc.Name );
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PixelShaderConstant< TNode >* DX9PixelShaderConstantsCompiler< TNode >::createObjectConstant( const D3DXCONSTANT_DESC& desc ) const
{
   switch( desc.Type )
   {
   case D3DXPT_TEXTURE:          // fallthrough
   case D3DXPT_TEXTURE1D:        // fallthrough
   case D3DXPT_TEXTURE2D:        // fallthrough
   case D3DXPT_TEXTURE3D:        // fallthrough
   case D3DXPT_TEXTURECUBE:      // fallthrough
   case D3DXPT_SAMPLER:          // fallthrough
   case D3DXPT_SAMPLER1D:        // fallthrough
   case D3DXPT_SAMPLER2D:        // fallthrough
   case D3DXPT_SAMPLER3D:        // fallthrough
   case D3DXPT_SAMPLERCUBE:
      {
         return new PSCTexture< TNode >( desc.Name );
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PixelShaderConstant< TNode >* DX9PixelShaderConstantsCompiler< TNode >::createMatrixConstant( const D3DXCONSTANT_DESC& desc ) const
{
   if ( desc.Type == D3DXPT_FLOAT )
   {
      return new PSCMatrix< TNode >( desc.Name );
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PixelShaderConstant< TNode >* DX9PixelShaderConstantsCompiler< TNode >::createConstant( const D3DXCONSTANT_DESC& desc ) const
{
   switch( desc.Class )
   {
   case D3DXPC_SCALAR:
      {
         return createScalarConstant( desc );
      }

   case D3DXPC_VECTOR:
      {
         return new PSCVec4< TNode >( desc.Name );
      }

   case D3DXPC_MATRIX_ROWS:
      {
         break;
      }

   case D3DXPC_MATRIX_COLUMNS:
      {
         return createMatrixConstant( desc );
      }

   case D3DXPC_OBJECT:
      {
         return createObjectConstant( desc );
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
bool DX9PixelShaderConstantsCompiler< TNode >::compile( const std::string& shaderCode, const char* entryFunction, std::vector< typename PixelShaderConstant< TNode >* >& outConstants )
{
   // create a renderer
   IDirect3D9* d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
   DX9Initializer initializer( *d3d9 );

   IDirect3DDevice9* compileDevice = initializer.createNullDevice();
   const char* shaderProfile = D3DXGetPixelShaderProfile( compileDevice );

   DWORD flags = D3DXSHADER_DEBUG;

   ID3DXBuffer* shaderBuf = NULL;
   ID3DXBuffer* errorsBuf = NULL;
   ID3DXConstantTable* shaderConstants = NULL;
   DX9ShaderIncludeLoader includesLoader;
   HRESULT res = D3DXCompileShader( shaderCode.c_str(), shaderCode.length(), NULL, &includesLoader, entryFunction, shaderProfile, flags, &shaderBuf, &errorsBuf, &shaderConstants );

   // interpret the results
   bool result = true;
   if ( FAILED(res) || shaderBuf == NULL )
   {
      if ( errorsBuf != NULL )
      {
         std::string compilationErrors = ( const char* )errorsBuf->GetBufferPointer();
         errorsBuf->Release();
         m_errorMsg = std::string( "Shader compilation error: " ) + compilationErrors;
      }
      else
      {
         m_errorMsg = translateDxError( "Error while compiling a shader", res );
      }

      result = false;
   }
   else
   {
      std::vector< D3DXCONSTANT_DESC > constants;
      result = parseConstants( shaderConstants, m_errorMsg, constants );
      if ( result )
      {
         unsigned int constantsCount = constants.size();
         for ( unsigned int i = 0; i < constantsCount; ++i )
         {
            PixelShaderConstant< TNode >* constant = createConstant( constants[i] );
            if ( constant != NULL )
            {
               outConstants.push_back( constant );
            }
         }
      }
   }

   // cleanup
   if ( shaderConstants )
   {
      shaderConstants->Release();
   }
   compileDevice->Release();
   d3d9->Release();

   return result;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _DX9_PIXEL_SHADER_CONSTANTS_COMPILER_H
