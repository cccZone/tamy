/// @file   core-Renderer/ShaderCompiler.h
/// @brief  a tool for compiling shader code fro verification purposes
#pragma once

#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class PixelShaderConstant;

///////////////////////////////////////////////////////////////////////////////

class ShaderCompiler
{
private:
   std::string    m_errorMsg;

public:
   /**
    * Compiles a pixel shader.
    *
    * @param shaderCode
    * @param entryFunction
    * @param outConstants        shader constants - pass NULL if you're not interested in them
    *
    * @return  'true' if the shader compiles OK, 'false' if any errors were encountered.
    *          In the latter case, check the `getLastError` method for the exact error message.
    */
   bool compilePixelShader( const std::string& shaderCode, const char* entryFunction, std::vector< PixelShaderConstant* >* outConstants = NULL );

   /**
    * Returns the last error message.
    */
   inline const std::string& getLastError() const { return m_errorMsg; }
};

///////////////////////////////////////////////////////////////////////////////
