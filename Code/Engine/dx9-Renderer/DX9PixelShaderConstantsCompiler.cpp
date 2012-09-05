#include "dx9-Renderer/DX9PixelShaderConstantsCompiler.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "dx9-Renderer/DX9ShaderIncludeLoader.h"
#include "dx9-Renderer/DXErrorParser.h"
#include "dx9-Renderer/DX9Initializer.h"
#include "dx9-Renderer/ShaderCompilerUtils.h"


///////////////////////////////////////////////////////////////////////////////

bool DX9PixelShaderConstantsCompiler::createScalarConstant( const D3DXCONSTANT_DESC& desc, ShaderConstantDesc& outDesc ) const
{
   switch( desc.Type )
   {
   case D3DXPT_BOOL:
      {
         outDesc.m_type = ShaderConstantDesc::CT_Bool;
         outDesc.m_name = desc.Name;
         return true;
      }

   case D3DXPT_INT:
      {
         outDesc.m_type = ShaderConstantDesc::CT_Int;
         outDesc.m_name = desc.Name;
         return true;
      }

   case D3DXPT_FLOAT:
      {
         outDesc.m_type = ShaderConstantDesc::CT_Float;
         outDesc.m_name = desc.Name;
         return true;
      }

   case D3DXPT_STRING:
      {
         outDesc.m_type = ShaderConstantDesc::CT_String;
         outDesc.m_name = desc.Name;
         return true;
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool DX9PixelShaderConstantsCompiler::createObjectConstant( const D3DXCONSTANT_DESC& desc, ShaderConstantDesc& outDesc ) const
{
   if ( ShaderCompilerUtils::isTexture( desc ) )
   {
      outDesc.m_type = ShaderConstantDesc::CT_Texture;
      outDesc.m_name = desc.Name;
      return true;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool DX9PixelShaderConstantsCompiler::createMatrixConstant( const D3DXCONSTANT_DESC& desc, ShaderConstantDesc& outDesc ) const
{
   if ( desc.Type == D3DXPT_FLOAT )
   {
      outDesc.m_type = ShaderConstantDesc::CT_Matrix;
      outDesc.m_name = desc.Name;
      return true;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool DX9PixelShaderConstantsCompiler::createConstant( const D3DXCONSTANT_DESC& desc, ShaderConstantDesc& outDesc ) const
{
   switch( desc.Class )
   {
   case D3DXPC_SCALAR:
      {
         return createScalarConstant( desc, outDesc );
      }

   case D3DXPC_VECTOR:
      {
         outDesc.m_type = ShaderConstantDesc::CT_Vec4;
         outDesc.m_name = desc.Name;
         return true;
      }

   case D3DXPC_MATRIX_ROWS:
      {
         break;
      }

   case D3DXPC_MATRIX_COLUMNS:
      {
         return createMatrixConstant( desc, outDesc );
      }

   case D3DXPC_OBJECT:
      {
         return createObjectConstant( desc, outDesc );
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool DX9PixelShaderConstantsCompiler::compile( const std::string& shaderCode, const char* entryFunction, std::vector< ShaderConstantDesc >& outConstants )
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
      ShaderConstantDesc newConstant;
      result = ShaderCompilerUtils::parseConstants( shaderConstants, m_errorMsg, constants );
      if ( result )
      {
         unsigned int constantsCount = constants.size();
         for ( unsigned int i = 0; i < constantsCount; ++i )
         {
            if ( createConstant( constants[i], newConstant ) )
            {
               outConstants.push_back( newConstant );
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
