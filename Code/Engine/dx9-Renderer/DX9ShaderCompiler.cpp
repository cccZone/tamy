#include "core-Renderer/ShaderCompiler.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "dx9-Renderer/DX9ShaderIncludeLoader.h"
#include "dx9-Renderer/DXErrorParser.h"
#include "dx9-Renderer/DX9Initializer.h"
#include "dx9-Renderer/DX9ShaderConstantsCompiler.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/MaterialNode.h"
#include <d3dx9.h>
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

static bool parseConstants( ID3DXConstantTable* shaderConstants, std::string& errMsg, std::vector< D3DXCONSTANT_DESC >& outConstants )
{
   // parse shader constants
   D3DXCONSTANTTABLE_DESC constantsDesc;
   HRESULT res = shaderConstants->GetDesc( &constantsDesc );
   if ( FAILED( res ) )
   {
      errMsg = translateDxError( "Can't access shader constants table", res );
      return false;
   }
   
   // iterate over all constants 
   for ( unsigned int i = 0; i < constantsDesc.Constants; ++i )
   {
      D3DXHANDLE constHandle = shaderConstants->GetConstant( NULL, i );

      D3DXCONSTANT_DESC constDesc[64];
      unsigned int constantsCount = 64;
      res = shaderConstants->GetConstantDesc( constHandle, constDesc, &constantsCount );
      if ( FAILED( res ) )
      {
         errMsg = translateDxError( "Error retrieving a shader constant", res );
         return false;
      }

      // now that we have a description, create proper constants
      for ( unsigned int j = 0; j < constantsCount; ++j )
      {
         outConstants.push_back( constDesc[j] );
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

static bool isTexture( const D3DXCONSTANT_DESC& desc )
{
   if ( desc.Class == D3DXPC_OBJECT )
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
            return true;
         }
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool ShaderCompiler::compilePixelShader( const std::string& shaderCode, const char* entryFunction )
{
   // create a renderer
   IDirect3D9* d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
   DX9Initializer initializer( *d3d9 );

   IDirect3DDevice9* compileDevice = initializer.createNullDevice();
   const char* shaderProfile = D3DXGetPixelShaderProfile( compileDevice );

   DWORD flags = D3DXSHADER_DEBUG;

   ID3DXBuffer* shaderBuf = NULL;
   ID3DXBuffer* errorsBuf = NULL;
   DX9ShaderIncludeLoader includesLoader;
   HRESULT res = D3DXCompileShader( shaderCode.c_str(), shaderCode.length(), NULL, &includesLoader, entryFunction, shaderProfile, flags, &shaderBuf, &errorsBuf, NULL );

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


   // cleanup
   compileDevice->Release();
   d3d9->Release();

   return result;
}

///////////////////////////////////////////////////////////////////////////////

bool ShaderCompiler::compilePixelShaderTextureStages( const std::string& shaderCode, const char* entryFunction, std::vector< std::string >& outNames )
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
            const D3DXCONSTANT_DESC& desc = constants[i];
            if ( isTexture( desc ) )
            {
               if ( desc.RegisterIndex >= outNames.size() )
               {
                  outNames.resize( desc.RegisterIndex + 1 );
               }
               outNames[ desc.RegisterIndex ] = desc.Name;
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

template<>
bool ShaderCompiler::compilePixelShaderConstants( const std::string& shaderCode, const char* entryFunction, std::vector< typename PixelShaderConstant< RenderingPipelineNode >* >& outConstants )
{
   DX9ShaderConstantsCompiler< RenderingPipelineNode > constantsCompiler;
   bool result = constantsCompiler.compile( shaderCode, entryFunction, outConstants );

   m_errorMsg = constantsCompiler.m_errorMsg;
   return result;
}

///////////////////////////////////////////////////////////////////////////////

template<>
bool ShaderCompiler::compilePixelShaderConstants( const std::string& shaderCode, const char* entryFunction, std::vector< typename PixelShaderConstant< MaterialNode >* >& outConstants )
{
   DX9ShaderConstantsCompiler< MaterialNode > constantsCompiler;
   bool result = constantsCompiler.compile( shaderCode, entryFunction, outConstants );

   m_errorMsg = constantsCompiler.m_errorMsg;
   return result;
}

///////////////////////////////////////////////////////////////////////////////

bool ShaderCompiler::compileVertexShader( const std::string& shaderCode, const char* entryFunction )
{
   // create a renderer
   IDirect3D9* d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
   DX9Initializer initializer( *d3d9 );

   IDirect3DDevice9* compileDevice = initializer.createNullDevice();
   const char* shaderProfile = D3DXGetVertexShaderProfile( compileDevice );

   DWORD flags = D3DXSHADER_DEBUG;

   ID3DXBuffer* shaderBuf = NULL;
   ID3DXBuffer* errorsBuf = NULL;
   DX9ShaderIncludeLoader includesLoader;
   HRESULT res = D3DXCompileShader( shaderCode.c_str(), shaderCode.length(), NULL, &includesLoader, entryFunction, shaderProfile, flags, &shaderBuf, &errorsBuf, NULL );

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

   // cleanup
   compileDevice->Release();
   d3d9->Release();

   return result;
}

///////////////////////////////////////////////////////////////////////////////

