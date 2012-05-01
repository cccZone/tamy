#ifndef _VERTEX_SHADER_CONSTANT_H
#error "This file can only be included from VertexShaderConstant.h"
#else

#include "core-Renderer/VertexShaderConstant.h"
#include "core-Renderer/VertexShader.h"
#include "core/GraphBuilderSockets.h"
#include "core-Renderer/ShaderTexture.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename DataType >
const ReflectionType* TVertexShaderConstant< TNode, DataType >::getDataType() const
{
   ReflectionTypesRegistry& registry = ReflectionTypesRegistry::getInstance();
   return registry.find< DataType >();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VSCBool< TNode >::VSCBool( const char* name, bool defaultVal )
   : TVertexShaderConstant< TNode, bool >( name )
   , m_defaultVal( defaultVal )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void VSCBool< TNode >::setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, bool >& castInput = static_cast< const TGBNodeInput< TNode, bool >& >( input );
   bool val = castInput.getValue( data );
   comm.setBool( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VSCInt< TNode >::VSCInt( const char* name, int defaultVal )
   : TVertexShaderConstant< TNode, int >( name )
   , m_defaultVal( defaultVal )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void VSCInt< TNode >::setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, int >& castInput = static_cast< const TGBNodeInput< TNode, int >& >( input );
   int val = castInput.getValue( data );
   comm.setInt( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VSCFloat< TNode >::VSCFloat( const char* name, float defaultVal )
   : TVertexShaderConstant< TNode, float >( name )
   , m_defaultVal( defaultVal )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void VSCFloat< TNode >::setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, float >& castInput = static_cast< const TGBNodeInput< TNode, float >& >( input );
   float val = castInput.getValue( data );
   comm.setFloat( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VSCString< TNode >::VSCString( const char* name)
   : TVertexShaderConstant< TNode, std::string >( name )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void VSCString< TNode >::setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, std::string >& castInput = static_cast< const TGBNodeInput< TNode, std::string >& >( input );
   const std::string& val = castInput.getValue( data );
   comm.setString( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VSCTexture< TNode >::VSCTexture( const char* name )
   : TVertexShaderConstant< TNode, ShaderTexture >( name )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void VSCTexture< TNode >::setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodePtrInput< TNode, ShaderTexture >& castInput = static_cast< const TGBNodePtrInput< TNode, ShaderTexture >& >( input );
   ShaderTexture* val = castInput.getValue( data );
   if ( val )
   {
      comm.setTexture( getName(), *val );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VSCVec4< TNode >::VSCVec4( const char* name)
   : TVertexShaderConstant< TNode, D3DXVECTOR4 >( name )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void VSCVec4< TNode >::setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, D3DXVECTOR4 >& castInput = static_cast< const TGBNodeInput< TNode, D3DXVECTOR4 >& >( input );
   const D3DXVECTOR4& val = castInput.getValue( data );
   comm.setVec4( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VSCMatrix< TNode >::VSCMatrix( const char* name)
   : TVertexShaderConstant< TNode, D3DXMATRIX >( name )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void VSCMatrix< TNode >::setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, D3DXMATRIX >& castInput = static_cast< const TGBNodeInput< TNode, D3DXMATRIX >& >( input );
   const D3DXMATRIX& val = castInput.getValue( data );
   comm.setMtx( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _VERTEX_SHADER_CONSTANT_H
