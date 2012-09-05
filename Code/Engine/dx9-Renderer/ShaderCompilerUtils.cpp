#include "dx9-Renderer/ShaderCompilerUtils.h"
#include "dx9-Renderer/DXErrorParser.h"



///////////////////////////////////////////////////////////////////////////////

bool ShaderCompilerUtils::parseConstants( ID3DXConstantTable* shaderConstants, std::string& errMsg, std::vector< D3DXCONSTANT_DESC >& outConstants )
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

bool ShaderCompilerUtils::isTexture( const D3DXCONSTANT_DESC& desc )
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
