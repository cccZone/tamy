/// @file   core-Renderer/VertexShaderConstant.h
/// @brief  pixel shader constant definition
#ifndef _VERTEX_SHADER_CONSTANT_H
#define _VERTEX_SHADER_CONSTANT_H

#include <string>
#include "core/ReflectionType.h"
#include "core/GraphBuilderSockets.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class RCBindVertexShader;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Vertex shader constant definition, required by the RenderingPipeline
 * in order to parametrize its nodes.
 */
template< typename TNode >
class VertexShaderConstant
{
private:
   std::string       m_name;

public:
   virtual ~VertexShaderConstant() {}

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
   virtual void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data ) = 0;

protected:
   /**
    * Constructor.
    *
    * @param name
    */
   VertexShaderConstant( const char* name ) : m_name( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type-specific vertex shader constant.
 */
template< typename TNode, typename DataType >
class TVertexShaderConstant : public VertexShaderConstant< TNode >
{
public:
   virtual ~TVertexShaderConstant() {}

   // -------------------------------------------------------------------------
   // VertexShaderConstant implementation
   // -------------------------------------------------------------------------
   const ReflectionType* getDataType() const;

protected:
   /**
    * Constructor.
    *
    * @param name
    */
   TVertexShaderConstant( const char* name ) : VertexShaderConstant( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar bool constant.
 */
template< typename TNode >
class VSCBool : public TVertexShaderConstant< TNode, bool >
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
   VSCBool( const char* name, bool defaultVal );

   // -------------------------------------------------------------------------
   // VertexShaderConstant implementation
   // -------------------------------------------------------------------------
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar int constant.
 */
template< typename TNode >
class VSCInt : public TVertexShaderConstant< TNode, int >
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
   VSCInt( const char* name, int defaultVal );

   // -------------------------------------------------------------------------
   // VertexShaderConstant implementation
   // -------------------------------------------------------------------------
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar float constant.
 */
template< typename TNode >
class VSCFloat : public TVertexShaderConstant< TNode, float >
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
   VSCFloat( const char* name, float defaultVal );

   // -------------------------------------------------------------------------
   // VertexShaderConstant implementation
   // -------------------------------------------------------------------------
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar string constant.
 */
template< typename TNode >
class VSCString : public TVertexShaderConstant< TNode, std::string >
{
public:
   /**
    * Constructor.
    *
    * @param name
    */
   VSCString( const char* name );

   // -------------------------------------------------------------------------
   // VertexShaderConstant implementation
   // -------------------------------------------------------------------------
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar texture constant.
 */
template< typename TNode >
class VSCTexture : public TVertexShaderConstant< TNode, ShaderTexture >
{
public:
   /**
    * Constructor.
    *
    * @param name
    */
   VSCTexture( const char* name );

   // -------------------------------------------------------------------------
   // VertexShaderConstant implementation
   // -------------------------------------------------------------------------
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar vector constant.
 */
template< typename TNode >
class VSCVec4 : public TVertexShaderConstant< TNode, D3DXVECTOR4 >
{
public:
   /**
    * Constructor.
    *
    * @param name
    */
   VSCVec4( const char* name );

   // -------------------------------------------------------------------------
   // VertexShaderConstant implementation
   // -------------------------------------------------------------------------
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Matrix4x4 constant.
 */
template< typename TNode >
class VSCMatrix : public TVertexShaderConstant< TNode, D3DXMATRIX >
{
public:
   /**
    * Constructor.
    *
    * @param name
    */
   VSCMatrix( const char* name );

   // -------------------------------------------------------------------------
   // VertexShaderConstant implementation
   // -------------------------------------------------------------------------
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer/VertexShaderConstant.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _VERTEX_SHADER_CONSTANT_H
