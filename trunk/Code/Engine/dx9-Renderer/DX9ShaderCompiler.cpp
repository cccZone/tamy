#include "core-Renderer/ShaderCompiler.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "dx9-Renderer/DX9ShaderIncludeLoader.h"
#include "dx9-Renderer/DXErrorParser.h"
#include "dx9-Renderer/DX9Initializer.h"
#include "core-Renderer/PixelShaderConstant.h"
#include <d3dx9.h>
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

static PixelShaderConstant* createScalarConstant( const D3DXCONSTANT_DESC& desc )
{
   switch( desc.Type )
   {
   case D3DXPT_BOOL:
      {
         return new PSCBool( desc.Name, false );
      }

   case D3DXPT_INT:
      {
         return new PSCInt( desc.Name, 0 );
      }

   case D3DXPT_FLOAT:
      {
         return new PSCFloat( desc.Name, 0.0f );
      }

   case D3DXPT_STRING:
      {
         return new PSCString( desc.Name );
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

static PixelShaderConstant* createObjectConstant( const D3DXCONSTANT_DESC& desc )
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
         return new PSCTexture( desc.Name );

         // TODO: parse the sampler settings and set the shader params structure accordingly
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

static PixelShaderConstant* createMatrixConstant( const D3DXCONSTANT_DESC& desc )
{
   if ( desc.Type == D3DXPT_FLOAT )
   {
      return new PSCMatrix( desc.Name );
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

static PixelShaderConstant* createConstant( const D3DXCONSTANT_DESC& desc )
{
   switch( desc.Class )
   {
   case D3DXPC_SCALAR:
      {
         return createScalarConstant( desc );
      }

   case D3DXPC_VECTOR:
      {
         return new PSCVec4( desc.Name );
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

static bool parseConstants( ID3DXConstantTable* shaderConstants, std::string& errMsg, std::vector< PixelShaderConstant* >& outConstants )
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
         const D3DXCONSTANT_DESC& desc = constDesc[j];
         PixelShaderConstant* constant = createConstant( desc );
         if ( constant )
         {
            outConstants.push_back( constant );
         }
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool ShaderCompiler::compilePixelShader( const std::string& shaderCode, const char* entryFunction, std::vector< PixelShaderConstant* >* outConstants )
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
      if ( outConstants )
      {
         int inputConstantsVecSize = outConstants->size();

         result = parseConstants( shaderConstants, m_errorMsg, *outConstants );

         if ( result == false )
         {
            // release the allocated constants, and only those that were allocated by us - the user
            // might have passed a vector that was already filled with constants definitions, and we can't touch those
            int count = outConstants->size();
            for ( int i = count - 1; i >= inputConstantsVecSize; --i )
            {
               delete outConstants->back();
               outConstants->pop_back();
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
