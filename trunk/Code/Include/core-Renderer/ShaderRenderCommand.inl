#ifndef _SHADER_RENDER_COMMAND_H
#error "This file can only be included from ShaderRenderCommand.h"
#else

#include "core-Renderer\ShaderParam.h"


///////////////////////////////////////////////////////////////////////////////

template< typename T >
ShaderRenderCommand< T >::~ShaderRenderCommand()
{
   unsigned int count = m_shaderParams.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_shaderParams[i];
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
void ShaderRenderCommand< T >::setBool( const std::string& paramName, bool val )
{
   m_shaderParams.push_back( new ShaderParamBool< T >( paramName, val ) );
} 

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setInt( const std::string& paramName, int val )
{
   m_shaderParams.push_back( new ShaderParamInt< T >( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setInt( const std::string& paramName, const int* arr, unsigned int size )
{
   m_shaderParams.push_back( new ShaderParamIntArray< T >( paramName, arr, size ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setFloat( const std::string& paramName, float val )
{
   m_shaderParams.push_back( new ShaderParamFloat< T >( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setFloat( const std::string& paramName, const float* arr, unsigned int size )
{
   m_shaderParams.push_back( new ShaderParamFloatArray< T >( paramName, arr, size ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setMtx( const std::string& paramName, const D3DXMATRIX& val )
{
   m_shaderParams.push_back( new ShaderParamMtx< T >( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setMtx( const std::string& paramName, const D3DXMATRIX* arr, unsigned int size )
{
   m_shaderParams.push_back( new ShaderParamMtxArray< T >( paramName, arr, size ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setString( const std::string& paramName, const std::string& val )
{
   m_shaderParams.push_back( new ShaderParamString< T >( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setTexture( const std::string& paramName, ShaderTexture& val )
{
   // we're using the good old Visitor pattern here to get around that we can't 
   // have template virtual methods - so we'll make our resources introduce
   // themselves to the ShaderTexture by calling a proper method on it,
   // and it will be the shader texture's responsiblity to instantiate
   // a proper param setter.
   m_shaderParams.push_back( T::createTextureSetter( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setVec4( const std::string& paramName, const D3DXVECTOR4& val )
{
   m_shaderParams.push_back( new ShaderParamVec4< T >( paramName, val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ShaderRenderCommand< T >::setVec4( const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size )
{
   m_shaderParams.push_back( new ShaderParamVec4Array< T >( paramName, arr, size ) );
}

///////////////////////////////////////////////////////////////////////////////

#endif _SHADER_RENDER_COMMAND_H
