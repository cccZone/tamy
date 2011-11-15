#include "core-Renderer/ShaderCompiler.h"
#include "dx9-Renderer/DX9ShaderIncludeLoader.h"
#include "dx9-Renderer/DXErrorParser.h"
#include "dx9-Renderer/DX9Initializer.h"
#include <d3dx9.h>
#include <d3d9.h>


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
   HRESULT res = D3DXCompileShader(
      shaderCode.c_str(), 
      shaderCode.length(),
      NULL,                            // defines
      &includesLoader,
      entryFunction,
      shaderProfile, 
      flags,
      &shaderBuf, 
      &errorsBuf,
      NULL );

   // cleanup
   compileDevice->Release();
   d3d9->Release();

   // interpret the results
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

      return false;
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////
