/// @file   core-Renderer/PixelShaderConstant.h
/// @brief  pixel shader constant definition
#ifndef _PIXEL_SHADER_CONSTANT_H
#define _PIXEL_SHADER_CONSTANT_H

#include <string>
#include "core/Class.h"
#include "core/GraphBuilderSockets.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class RCBindPixelShader;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Pixel shader constant definition, required by the RenderingPipeline
 * in order to parametrize its nodes.
 */
template< typename TNode >
class PixelShaderConstant
{
private:
   std::string       m_name;

public:
   virtual ~PixelShaderConstant() {}

   /**
    * Returns the name assigned to an input of the RenderingPipelineNode that uses this constant.
    */
   const std::string& getName() const { return m_name; }

   /**
    * Checks if the existing input node has a compliant type with the one the constant required.
    *
    * @param input
    */
   virtual Class getDataType() const = 0;

   /**
    * Sets the constant value on the specified render command. The value
    * comes from the specified input.
    *
    * @param comm
    * @param input
    * @param data
    */
   virtual void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data ) = 0;

protected:
   /**
    * Constructor.
    *
    * @param name
    */
   PixelShaderConstant( const char* name ) : m_name( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar bool constant.
 */
template< typename TNode >
class PSCBool : public PixelShaderConstant< TNode >
{
private:
   bool           m_defaultVal;

public:
   /**
    * Constructor.
    *
    * @param name
    * @param defaultVal
    */
   PSCBool( const char* name, bool defaultVal );

   // -------------------------------------------------------------------------
   // PixelShaderConstant implementation
   // -------------------------------------------------------------------------
   Class getDataType() const { return Class::createClass< bool >(); }
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar int constant.
 */
template< typename TNode >
class PSCInt : public PixelShaderConstant< TNode >
{
private:
   int           m_defaultVal;

public:
   /**
    * Constructor.
    *
    * @param name
    * @param defaultVal
    */
   PSCInt( const char* name, int defaultVal );

   // -------------------------------------------------------------------------
   // PixelShaderConstant implementation
   // -------------------------------------------------------------------------
   Class getDataType() const { return Class::createClass< int >(); }
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar float constant.
 */
template< typename TNode >
class PSCFloat : public PixelShaderConstant< TNode >
{
private:
   float           m_defaultVal;

public:
   /**
    * Constructor.
    *
    * @param name
    * @param defaultVal
    */
   PSCFloat( const char* name, float defaultVal );

   // -------------------------------------------------------------------------
   // PixelShaderConstant implementation
   // -------------------------------------------------------------------------
   Class getDataType() const { return Class::createClass< float >(); }
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar string constant.
 */
template< typename TNode >
class PSCString : public PixelShaderConstant< TNode >
{
public:
   /**
    * Constructor.
    *
    * @param name
    */
   PSCString( const char* name );

   // -------------------------------------------------------------------------
   // PixelShaderConstant implementation
   // -------------------------------------------------------------------------
   Class getDataType() const { return Class::createClass< std::string >(); }
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar texture constant.
 */
template< typename TNode >
class PSCTexture : public PixelShaderConstant< TNode >
{
public:
   /**
    * Constructor.
    *
    * @param name
    */
   PSCTexture( const char* name );

   // -------------------------------------------------------------------------
   // PixelShaderConstant implementation
   // -------------------------------------------------------------------------
   Class getDataType() const { return Class::createClass< ShaderTexture >(); }
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar vector constant.
 */
template< typename TNode >
class PSCVec4 : public PixelShaderConstant< TNode >
{
public:
   /**
    * Constructor.
    *
    * @param name
    */
   PSCVec4( const char* name );

   // -------------------------------------------------------------------------
   // PixelShaderConstant implementation
   // -------------------------------------------------------------------------
   Class getDataType() const { return Class::createClass< D3DXVECTOR4 >(); }
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Matrix4x4 constant.
 */
template< typename TNode >
class PSCMatrix : public PixelShaderConstant< TNode >
{
public:
   /**
    * Constructor.
    *
    * @param name
    */
   PSCMatrix( const char* name );

   // -------------------------------------------------------------------------
   // PixelShaderConstant implementation
   // -------------------------------------------------------------------------
   Class getDataType() const { return Class::createClass< D3DXMATRIX >(); }
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer/PixelShaderConstant.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _PIXEL_SHADER_CONSTANT_H
