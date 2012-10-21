/// @file   core-Renderer/PixelShaderConstant.h
/// @brief  pixel shader constant definition
#ifndef _PIXEL_SHADER_CONSTANT_H
#define _PIXEL_SHADER_CONSTANT_H

#include <string>
#include "core/ReflectionType.h"
#include "core/GraphBuilderSockets.h"
#include "core-Renderer/ShaderTexture.h"
#include "core/MemoryRouter.h"
#include "core/Math.h"


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
   virtual const ReflectionType* getDataType() const = 0;

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
 * Type-specific pixel shader constant.
 */
template< typename TNode, typename DataType >
class TPixelShaderConstant : public PixelShaderConstant< TNode >
{
   DECLARE_ALLOCATOR( TPixelShaderConstant, AM_DEFAULT );

public:
   virtual ~TPixelShaderConstant() {}

   // -------------------------------------------------------------------------
   // PixelShaderConstant implementation
   // -------------------------------------------------------------------------
   const ReflectionType* getDataType() const;

protected:
   /**
    * Constructor.
    *
    * @param name
    */
   TPixelShaderConstant( const char* name ) : PixelShaderConstant( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar bool constant.
 */
template< typename TNode >
class PSCBool : public TPixelShaderConstant< TNode, bool >
{
   DECLARE_ALLOCATOR( PSCBool, AM_DEFAULT );

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
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar int constant.
 */
template< typename TNode >
class PSCInt : public TPixelShaderConstant< TNode, int >
{
   DECLARE_ALLOCATOR( PSCInt, AM_DEFAULT );

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
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar float constant.
 */
template< typename TNode >
class PSCFloat : public TPixelShaderConstant< TNode, float >
{
   DECLARE_ALLOCATOR( PSCFloat, AM_DEFAULT );

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
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar string constant.
 */
template< typename TNode >
class PSCString : public TPixelShaderConstant< TNode, std::string >
{
   DECLARE_ALLOCATOR( PSCString, AM_DEFAULT );

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
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar texture constant.
 */
template< typename TNode >
class PSCTexture : public TPixelShaderConstant< TNode, ShaderTexture >
{
   DECLARE_ALLOCATOR( PSCTexture, AM_DEFAULT );

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
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar vector constant.
 */
template< typename TNode >
class PSCVec4 : public TPixelShaderConstant< TNode, Vector >
{
   DECLARE_ALLOCATOR( PSCVec4, AM_DEFAULT );

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
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Matrix4x4 constant.
 */
template< typename TNode >
class PSCMatrix : public TPixelShaderConstant< TNode, Matrix >
{
   DECLARE_ALLOCATOR( PSCMatrix, AM_DEFAULT );

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
   void setValue( RCBindPixelShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer/PixelShaderConstant.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _PIXEL_SHADER_CONSTANT_H
