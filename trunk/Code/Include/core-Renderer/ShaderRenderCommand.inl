#ifndef _SHADER_RENDER_COMMAND_H
#error "This file can only be included from ShaderRenderCommand.h"
#else

#include "core-Renderer\ShaderParam.h"
#include "core\MemoryPoolAllocator.h"
#include "core\IDString.h"


///////////////////////////////////////////////////////////////////////////////

template< typename T >
ShaderRenderCommand< T >::ShaderRenderCommand( MemoryPoolAllocator& allocator )
   : m_allocator( allocator )
   , m_shaderParams( 16 ) // TODO: !!!!!!!!!!!! MAKE ARRAYS ALLOCATOR FRIENDLY AND USE ONE IN HERE !!!!!!!!!!!!!!
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
ShaderRenderCommand< T >::~ShaderRenderCommand()
{
   unsigned int count = m_shaderParams.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_allocator.destroy( m_shaderParams[i] );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setParams( Renderer& renderer, void* shaderImpl )
{
   unsigned int count = m_shaderParams.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_shaderParams[i]->setParam( renderer, shaderImpl );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setBool( const IDString& paramName, bool val )
{
   m_shaderParams.push_back( new ( m_allocator ) ShaderParamBool< T >( paramName, val ) );
} 

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setInt( const IDString& paramName, int val )
{
   m_shaderParams.push_back( new ( m_allocator ) ShaderParamInt< T >( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setInt( const IDString& paramName, const int* arr, unsigned int size )
{
   m_shaderParams.push_back( new ( m_allocator ) ShaderParamIntArray< T >( paramName, arr, size ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setFloat( const IDString& paramName, float val )
{
   m_shaderParams.push_back( new ( m_allocator ) ShaderParamFloat< T >( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setFloat( const IDString& paramName, const float* arr, unsigned int size )
{
   m_shaderParams.push_back( new ( m_allocator ) ShaderParamFloatArray< T >( paramName, arr, size ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setMtx( const IDString& paramName, const Matrix& val )
{
   m_shaderParams.push_back( new ( m_allocator ) ShaderParamMtx< T >( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setMtx( const IDString& paramName, const Matrix* arr, unsigned int size )
{
   m_shaderParams.push_back( new ( m_allocator ) ShaderParamMtxArray< T >( paramName, arr, size ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setString( const IDString& paramName, const std::string& val )
{
   m_shaderParams.push_back( new ( m_allocator ) ShaderParamString< T >( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setTexture( const IDString& paramName, ShaderTexture& val )
{
   // we're using the good old Visitor pattern here to get around that we can't 
   // have template virtual methods - so we'll make our resources introduce
   // themselves to the ShaderTexture by calling a proper method on it,
   // and it will be the shader texture's responsibility to instantiate
   // a proper param setter.
   m_shaderParams.push_back( T::createTextureSetter( m_allocator, paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setVec4( const IDString& paramName, const Vector& val )
{
   m_shaderParams.push_back( new ( m_allocator ) ShaderParamVec4< T >( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setVec4( const IDString& paramName, const Vector* arr, unsigned int size )
{
   m_shaderParams.push_back( new ( m_allocator ) ShaderParamVec4Array< T >( paramName, arr, size ) );
}

///////////////////////////////////////////////////////////////////////////////

#endif _SHADER_RENDER_COMMAND_H
