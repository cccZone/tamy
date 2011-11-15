/// @file   core-Renderer/ShaderCompiler.h
/// @brief  a tool for compiling shader code fro verification purposes
#pragma once

#include <string>


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
    * @return  'true' if the shader compiles OK, 'false' if any errors were encountered.
    *          In the latter case, check the `getLastError` method for the exact error message.
    */
   bool compilePixelShader( const std::string& shaderCode, const char* entryFunction );

   /**
    * Returns the last error message.
    */
   inline const std::string& getLastError() const { return m_errorMsg; }
};

///////////////////////////////////////////////////////////////////////////////
