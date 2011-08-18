/// @file   core-Renderer/ShaderRenderCommands.h
/// @brief  common base for shader rendering commands
#ifndef _SHADER_RENDER_COMMAND_H
#define _SHADER_RENDER_COMMAND_H

#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\ShaderParam.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class ShaderTexture;

///////////////////////////////////////////////////////////////////////////////

/**
 * A common base for shader rendering commands.
 */
template< typename T >
class ShaderRenderCommand : public RenderCommand 
{
private:
   Array< ShaderParam< T >* >      m_shaderParams;

public:
   virtual ~ShaderRenderCommand();

   void setBool( const std::string& paramName, bool val );
   void setInt( const std::string& paramName, int val );
   void setInt( const std::string& paramName, const int* arr, unsigned int size );
   void setFloat( const std::string& paramName, float val );
   void setFloat( const std::string& paramName, const float* arr, unsigned int size );
   void setMtx( const std::string& paramName, const D3DXMATRIX& val );
   void setMtx( const std::string& paramName, const D3DXMATRIX* arr, unsigned int size );
   void setString( const std::string& paramName, const std::string& val );
   void setTexture( const std::string& paramName, ShaderTexture& val );
   void setVec4( const std::string& paramName, const D3DXVECTOR4& val );
   void setVec4( const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size );

protected:
   /**
    * Call this to set the parameters on an implementation-specific shader.
    *
    * @param renderer         host renderer
    * @param shaderImpl       pointer to a platform specific shader implementation
    */
   void setParams( Renderer& renderer, void* shaderImpl );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer/ShaderRenderCommand.inl"

///////////////////////////////////////////////////////////////////////////////

#endif _SHADER_RENDER_COMMAND_H
