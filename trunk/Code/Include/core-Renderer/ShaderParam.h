/// @file   core-Renderer/ShaderParam.h
/// @brief  class that sets specific shader parameter
#ifndef _SHADER_PARAM_H
#define _SHADER_PARAM_H

#include <string>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class EffectShader;
class VertexShader;
class PixelShader;
class Texture;
class RenderTarget;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Class that sets specific shader parameter. Visitor pattern used.
 */
template< typename T >
class ShaderParam
{
public:
   virtual ~ShaderParam() {}

   /**
    * Sets the parameter on a shader implementation.
    *
    * @param renderer         host renderer
    * @param shaderPtr        pointer to an implementation specific shader - reinterpret_cast it
    */
   virtual void setParam( Renderer& renderer, void* shaderPtr ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

// TODO: OPTIMIZATION candidate - replace strings in the params with some enum

///////////////////////////////////////////////////////////////////////////////

/**
 * Boolean shader parameter.
 */
template< typename T >
class ShaderParamBool : public ShaderParam< T >
{
private:
   std::string          m_name;
   bool                 m_val;

public:
   ShaderParamBool( const std::string& name, bool val );

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Int shader parameter.
 */
template< typename T >
class ShaderParamInt : public ShaderParam< T >
{
private:
   std::string          m_name;
   int                  m_val;

public:
   ShaderParamInt( const std::string& name, int val );

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Int array shader parameter.
 */
template< typename T >
class ShaderParamIntArray : public ShaderParam< T >
{
private:
   std::string          m_name;
   int*                 m_val;
   unsigned int         m_size;

public:
   ShaderParamIntArray( const std::string& name, const int* arr, unsigned int size );
   ~ShaderParamIntArray();

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Float shader parameter.
 */
template< typename T >
class ShaderParamFloat : public ShaderParam< T >
{
private:
   std::string          m_name;
   float                m_val;

public:
   ShaderParamFloat( const std::string& name, float val );

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Float array shader parameter.
 */
template< typename T >
class ShaderParamFloatArray : public ShaderParam< T >
{
private:
   std::string          m_name;
   float*               m_val;
   unsigned int         m_size;

public:
   ShaderParamFloatArray( const std::string& name, const float* arr, unsigned int size );
   ~ShaderParamFloatArray();

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Matrix shader parameter.
 */
template< typename T >
class ShaderParamMtx : public ShaderParam< T >
{
private:
   std::string          m_name;
   D3DXMATRIX           m_val;

public:
   ShaderParamMtx( const std::string& name, const D3DXMATRIX& val );

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Float array shader parameter.
 */
template< typename T >
class ShaderParamMtxArray : public ShaderParam< T >
{
private:
   std::string          m_name;
   D3DXMATRIX*          m_val;
   unsigned int         m_size;

public:
   ShaderParamMtxArray( const std::string& name, const D3DXMATRIX* arr, unsigned int size );
   ~ShaderParamMtxArray();

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Vector4 shader parameter.
 */
template< typename T >
class ShaderParamVec4 : public ShaderParam< T >
{
private:
   std::string          m_name;
   D3DXVECTOR4          m_val;

public:
   ShaderParamVec4( const std::string& name, const D3DXVECTOR4& val );

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Float array shader parameter.
 */
template< typename T >
class ShaderParamVec4Array : public ShaderParam< T >
{
private:
   std::string          m_name;
   D3DXVECTOR4*         m_val;
   unsigned int         m_size;

public:
   ShaderParamVec4Array( const std::string& name, const D3DXVECTOR4* arr, unsigned int size );
   ~ShaderParamVec4Array();

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * String shader parameter.
 */
template< typename T >
class ShaderParamString : public ShaderParam< T >
{
private:
   std::string          m_name;
   std::string          m_val;

public:
   ShaderParamString( const std::string& name, const std::string& val );

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Texture shader parameter.
 */
template< typename T >
class ShaderParamTexture : public ShaderParam< T >
{
private:
   std::string          m_name;
   Texture&             m_val;

public:
   ShaderParamTexture( const std::string& name, Texture& val );

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Render target shader parameter.
 */
template< typename T >
class ShaderParamRenderTarget : public ShaderParam< T >
{
private:
   std::string          m_name;
   RenderTarget&        m_val;

public:
   ShaderParamRenderTarget( const std::string& name, RenderTarget& val );

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\ShaderParam.inl"

///////////////////////////////////////////////////////////////////////////////

#endif _SHADER_PARAM_H
