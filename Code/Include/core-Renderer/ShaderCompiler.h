/// @file   core-Renderer/ShaderCompiler.h
/// @brief  a tool for compiling shader code fro verification purposes
#pragma once

#include <string>
#include <vector>
#include "core-Renderer/PixelShaderConstant.h"


///////////////////////////////////////////////////////////////////////////////

class ShaderCompiler
{
private:
   std::string    m_errorMsg;

public:
   /**
    * Compiles a pixel shader for error messages.
    *
    * @param shaderCode
    * @param entryFunction
    *
    * @return  'true' if the shader compiles OK, 'false' if any errors were encountered.
    *          In the latter case, check the `getLastError` method for the exact error message.
    */
   bool compilePixelShader( const std::string& shaderCode, const char* entryFunction );

   /**
    * Compiles shader constants for the specific type of node.
    *
    * @param shaderCode
    * @param entryFunction
    * @param outConstants        shader constants - pass NULL if you're not interested in them
    *
    * @return  'true' if the shader compiles OK, 'false' if any errors were encountered.
    *          In the latter case, check the `getLastError` method for the exact error message.
    */
   template< typename TNode >
   bool compilePixelShaderConstants( const std::string& shaderCode, const char* entryFunction, std::vector< typename PixelShaderConstant< TNode >* >& outConstants );

   /**
    * Compiles a list of texture stages required by a pixel shader.
    *
    * @param shaderCode
    * @param entryFunction
    * @param outNames
    *
    * @return  'true' if the shader compiles OK, 'false' if any errors were encountered.
    *          In the latter case, check the `getLastError` method for the exact error message.
    */
   bool compilePixelShaderTextureStages( const std::string& shaderCode, const char* entryFunction, std::vector< std::string >& outNames );

   /**
    * Compiles a vertex shader.
    *
    * @param shaderCode
    * @param entryFunction
    *
    * @return  'true' if the shader compiles OK, 'false' if any errors were encountered.
    *          In the latter case, check the `getLastError` method for the exact error message.
    */
   bool compileVertexShader( const std::string& shaderCode, const char* entryFunction );

   /**
    * Returns the last error message.
    */
   inline const std::string& getLastError() const { return m_errorMsg; }
};

///////////////////////////////////////////////////////////////////////////////
