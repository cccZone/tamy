/// @file   core-Renderer/VertexShaderConstant.h
/// @brief  pixel shader constant definition
#ifndef _VERTEX_SHADER_CONSTANT_H
#define _VERTEX_SHADER_CONSTANT_H

#include <string>
#include "core/Class.h"
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
   virtual Class getDataType() const = 0;

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
 * Scalar bool constant.
 */
template< typename TNode >
class VSCBool : public VertexShaderConstant< TNode >
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
   Class getDataType() const { return Class::createClass< bool >(); }
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar int constant.
 */
template< typename TNode >
class VSCInt : public VertexShaderConstant< TNode >
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
   Class getDataType() const { return Class::createClass< int >(); }
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar float constant.
 */
template< typename TNode >
class VSCFloat : public VertexShaderConstant< TNode >
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
   Class getDataType() const { return Class::createClass< float >(); }
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar string constant.
 */
template< typename TNode >
class VSCString : public VertexShaderConstant< TNode >
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
   Class getDataType() const { return Class::createClass< std::string >(); }
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar texture constant.
 */
template< typename TNode >
class VSCTexture : public VertexShaderConstant< TNode >
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
   Class getDataType() const { return Class::createClass< ShaderTexture >(); }
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar vector constant.
 */
template< typename TNode >
class VSCVec4 : public VertexShaderConstant< TNode >
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
   Class getDataType() const { return Class::createClass< D3DXVECTOR4 >(); }
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Matrix4x4 constant.
 */
template< typename TNode >
class VSCMatrix : public VertexShaderConstant< TNode >
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
   Class getDataType() const { return Class::createClass< D3DXMATRIX >(); }
   void setValue( RCBindVertexShader& comm, const GBNodeInput< TNode >& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer/VertexShaderConstant.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _VERTEX_SHADER_CONSTANT_H
