/// @file   dx9-Renderer/ShaderCompilerUtils.h
/// @brief  various utility methods used by the shaders compilers
#pragma once

#include <vector>
#include <d3dx9.h>
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * Various utility methods used by the shaders compilers.
 */
class ShaderCompilerUtils
{
public:
   /**
    * Parses shader constants.
    *
    * @param shaderConstants
    * @param errMsg
    * @param outConstants
    */
   static bool parseConstants( ID3DXConstantTable* shaderConstants, std::string& errMsg, std::vector< D3DXCONSTANT_DESC >& outConstants );

   /**
    * Checks if the constant describes a texture.
    *
    * @param desc
    */
   static bool isTexture( const D3DXCONSTANT_DESC& desc );
};

///////////////////////////////////////////////////////////////////////////////
