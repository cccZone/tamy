/// @file   core-Renderer/PixelShaderConstant.h
/// @brief  pixel shader constant definition
#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class RPNodeInput;
class RCBindPixelShader;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Pixel shader constant definition, required by the RenderingPipeline
 * in order to parametrize its nodes.
 */
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
    * Creates an input that should be set in the RenderingPipelineNode, so this
    * constant can be fed proper data.
    */
   virtual RPNodeInput* createRPNInput() = 0;

   /**
    * Checks if the existing input node has a compliant type with the one the constant required.
    *
    * @param input
    */
   virtual bool doesTypeMatch( const RPNodeInput& input ) const = 0;

   /**
    * Sets the constant value on the specified render command. The value
    * comes from the specified input.
    *
    * @param comm
    * @param input
    * @param data
    */
   virtual void setValue( RCBindPixelShader& comm, const RPNodeInput& input, RuntimeDataBuffer& data ) = 0;

   /**
    * Returns the index of the required texture stage.
    */
   virtual int getRequiredTextureStageIdx() const { return -1; }

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
class PSCBool : public PixelShaderConstant
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
   RPNodeInput* createRPNInput();
   bool doesTypeMatch( const RPNodeInput& input ) const;
   void setValue( RCBindPixelShader& comm, const RPNodeInput& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar int constant.
 */
class PSCInt : public PixelShaderConstant
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
   RPNodeInput* createRPNInput();
  bool doesTypeMatch( const RPNodeInput& input ) const;
   void setValue( RCBindPixelShader& comm, const RPNodeInput& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar float constant.
 */
class PSCFloat : public PixelShaderConstant
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
   RPNodeInput* createRPNInput();
   bool doesTypeMatch( const RPNodeInput& input ) const;
   void setValue( RCBindPixelShader& comm, const RPNodeInput& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar string constant.
 */
class PSCString : public PixelShaderConstant
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
   RPNodeInput* createRPNInput();
   bool doesTypeMatch( const RPNodeInput& input ) const;
   void setValue( RCBindPixelShader& comm, const RPNodeInput& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar texture constant.
 */
class PSCTexture : public PixelShaderConstant
{
private:
   int      m_textureStageIdx;

public:
   /**
    * Constructor.
    *
    * @param name
    * @param textureStageIdx
    */
   PSCTexture( const char* name, int textureStageIdx );

   // -------------------------------------------------------------------------
   // PixelShaderConstant implementation
   // -------------------------------------------------------------------------
   RPNodeInput* createRPNInput();
   bool doesTypeMatch( const RPNodeInput& input ) const;
   void setValue( RCBindPixelShader& comm, const RPNodeInput& input, RuntimeDataBuffer& data );
   int getRequiredTextureStageIdx() const { return m_textureStageIdx; }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Scalar vector constant.
 */
class PSCVec4 : public PixelShaderConstant
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
   RPNodeInput* createRPNInput();
   bool doesTypeMatch( const RPNodeInput& input ) const;
   void setValue( RCBindPixelShader& comm, const RPNodeInput& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Matrix4x4 constant.
 */
class PSCMatrix : public PixelShaderConstant
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
   RPNodeInput* createRPNInput();
   bool doesTypeMatch( const RPNodeInput& input ) const;
   void setValue( RCBindPixelShader& comm, const RPNodeInput& input, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////
