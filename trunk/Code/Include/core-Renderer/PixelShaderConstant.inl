#ifndef _PIXEL_SHADER_CONSTANT_H
#error "This file can only be included from PixelShaderConstant.h"
#else

#include "core-Renderer/PixelShaderConstant.h"
#include "core-Renderer/PixelShader.h"
#include "core/GraphBuilderSockets.h"
#include "core-Renderer/ShaderTexture.h"


///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename DataType >
const ReflectionType* TPixelShaderConstant< TNode, DataType >::getDataType() const
{
   ReflectionTypesRegistry& registry = ReflectionTypesRegistry::getInstance();
   return registry.find< DataType >();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PSCBool< TNode >::PSCBool( const char* name, bool defaultVal )
   : TPixelShaderConstant< TNode, bool >( name )
   , m_defaultVal( defaultVal )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PSCBool< TNode >::setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, bool >& castInput = static_cast< const TGBNodeInput< TNode, bool >& >( input );
   bool val = castInput.getValue( data );
   comm.setBool( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PSCInt< TNode >::PSCInt( const char* name, int defaultVal )
   : TPixelShaderConstant< TNode, int >( name )
   , m_defaultVal( defaultVal )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PSCInt< TNode >::setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, int >& castInput = static_cast< const TGBNodeInput< TNode, int >& >( input );
   int val = castInput.getValue( data );
   comm.setInt( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PSCFloat< TNode >::PSCFloat( const char* name, float defaultVal )
   : TPixelShaderConstant< TNode, float >( name )
   , m_defaultVal( defaultVal )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PSCFloat< TNode >::setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, float >& castInput = static_cast< const TGBNodeInput< TNode, float >& >( input );
   float val = castInput.getValue( data );
   comm.setFloat( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PSCString< TNode >::PSCString( const char* name)
   : TPixelShaderConstant< TNode, std::string >( name )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PSCString< TNode >::setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, std::string >& castInput = static_cast< const TGBNodeInput< TNode, std::string >& >( input );
   const std::string& val = castInput.getValue( data );
   comm.setString( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PSCTexture< TNode >::PSCTexture( const char* name )
   : TPixelShaderConstant< TNode, ShaderTexture >( name )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PSCTexture< TNode >::setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodePtrInput< TNode, ShaderTexture >& castInput = static_cast< const TGBNodePtrInput< TNode, ShaderTexture >& >( input );
   ShaderTexture* val = castInput.getValue( data );
   if ( val )
   {
      comm.setTexture( getName(), val );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PSCVec4< TNode >::PSCVec4( const char* name )
   : TPixelShaderConstant< TNode, Vector >( name )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PSCVec4< TNode >::setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, Vector >& castInput = static_cast< const TGBNodeInput< TNode, Vector >& >( input );
   const Vector& val = castInput.getValue( data );
   comm.setVec4( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PSCMatrix< TNode >::PSCMatrix( const char* name)
   : TPixelShaderConstant< TNode, Matrix >( name )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PSCMatrix< TNode >::setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data )
{
   const TGBNodeInput< TNode, Matrix >& castInput = static_cast< const TGBNodeInput< TNode, Matrix >& >( input );
   const Matrix& val = castInput.getValue( data );
   comm.setMtx( getName(), val );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PIXEL_SHADER_CONSTANT_H
