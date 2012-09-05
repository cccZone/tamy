/// @file   core-Renderer/ShaderCompiler.h
/// @brief  a tool for compiling shader code for verification purposes
#pragma once

#include <string>
#include <vector>
#include "core-Renderer/PixelShaderConstant.h"
#include "core-Renderer/VertexShaderConstant.h"
#include "core/types.h"


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
   bool compilePixelShaderConstants( const std::string& shaderCode, const char* entryFunction, std::vector< ShaderConstantDesc >& outConstants );

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
    * Compiles shader constants for the specific type of node.
    *
    * @param shaderCode
    * @param entryFunction
    * @param outConstants        shader constants - pass NULL if you're not interested in them
    *
    * @return  'true' if the shader compiles OK, 'false' if any errors were encountered.
    *          In the latter case, check the `getLastError` method for the exact error message.
    */
   bool compileVertexShaderConstants( const std::string& shaderCode, const char* entryFunction, std::vector< ShaderConstantDesc >& outConstants );

   /**
    * Parses the techniques contained in the specified vertex shader code.
    *
    * Different techniques are actually the names of output structures of particular main functions the vertex
    * shader code contains.
    *
    * In order to tell whether the specified function is the vertex shader technique's main function
    * one needs to annotate it with a <technique> comment, like so:
    *
    *  // <technique>
    *  TECHNIQUE_STRUCT main(...)
    *
    * @return  'true' if the shader compiles OK, 'false' if any errors were encountered.
    *          In the latter case, check the `getLastError` method for the exact error message.
    */
   bool parseVertexShaderTechniques( const std::string& shaderCode, std::vector< std::string >& outTechniqueNames, std::vector< std::string >& outEntryFunctions );

   /**
    * Parses the vertex shader technique required by the specified pixel shader.
    *
    * It assumes the pixel shader main method declaration is properly annotated, and looks like this:
    *
    *  // <psMain>
    *  OUPTUT_STRUCT main( TECHNIQUE_STRUCT In )
    *
    * @param   shaderCode
    * @return  required technique name
    */
   std::string parseRequiredVertexShaderTechnique( const std::string& shaderCode );

   /**
    * Returns the last error message.
    */
   inline const std::string& getLastError() const { return m_errorMsg; }
};

///////////////////////////////////////////////////////////////////////////////
