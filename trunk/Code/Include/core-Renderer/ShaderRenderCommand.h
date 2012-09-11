/// @file   core-Renderer/ShaderRenderCommands.h
/// @brief  common base for shader rendering commands
#ifndef _SHADER_RENDER_COMMAND_H
#define _SHADER_RENDER_COMMAND_H

#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\ShaderParam.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class ShaderTexture;
class MemoryPoolAllocator;
class IDString;

///////////////////////////////////////////////////////////////////////////////

/**
 * A common base for shader rendering commands.
 */
template< typename T >
class ShaderRenderCommand : public RenderCommand 
{
private:
   MemoryPoolAllocator&                                  m_allocator;
   Array< ShaderParam< T >*, MemoryPoolAllocator >       m_shaderParams;

public:
   /**
    * Constructor.
    *
    * @param allocator
    */
   ShaderRenderCommand( MemoryPoolAllocator& allocator );
   virtual ~ShaderRenderCommand();

   void setBool( const IDString& paramName, bool val );
   void setInt( const IDString& paramName, int val );
   void setInt( const IDString& paramName, const int* arr, unsigned int size );
   void setFloat( const IDString& paramName, float val );
   void setFloat( const IDString& paramName, const float* arr, unsigned int size );
   void setMtx( const IDString& paramName, const Matrix& val );
   void setMtx( const IDString& paramName, const Matrix* arr, unsigned int size );
   void setString( const IDString& paramName, const std::string& val );
   void setTexture( const IDString& paramName, ShaderTexture* val );
   void setVec4( const IDString& paramName, const Vector& val );
   void setVec4( const IDString& paramName, const Vector* arr, unsigned int size );

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
