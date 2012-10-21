/// @file   core-Renderer/ShaderParam.h
/// @brief  class that sets specific shader parameter
#ifndef _SHADER_PARAM_H
#define _SHADER_PARAM_H

#include <string>
#include "core/types.h"
#include "core/MemoryRouter.h"
#include "core/IDString.h"


///////////////////////////////////////////////////////////////////////////////

struct Matrix;
struct Vector;
class EffectShader;
class VertexShader;
class PixelShader;
class Texture;
class RenderTarget;
class Renderer;
class MemoryPoolAllocator;

///////////////////////////////////////////////////////////////////////////////

/**
 * Class that sets specific shader parameter. Visitor pattern used.
 *
 * The class is designed to be instanced in a memory pool only,
 */
template< typename T >
class ShaderParam
{
   DECLARE_ALLOCATOR( ShaderParam, AM_DEFAULT );

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

/**
 * Boolean shader parameter.
 */
template< typename T >
class ShaderParamBool : public ShaderParam< T >
{
   DECLARE_ALLOCATOR( ShaderParamBool, AM_DEFAULT );

private:
   IDString             m_name;
   bool                 m_val;

public:
   ShaderParamBool( const IDString& nameId, bool val );

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
   DECLARE_ALLOCATOR( ShaderParamInt, AM_DEFAULT );

private:
   IDString             m_name;
   int                  m_val;

public:
   ShaderParamInt( const IDString& nameId, int val );

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
   DECLARE_ALLOCATOR( ShaderParamIntArray, AM_DEFAULT );

private:
   IDString             m_name;
   int*                 m_val;
   unsigned int         m_size;

public:
   ShaderParamIntArray( const IDString& nameId, const int* arr, unsigned int size );
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
   DECLARE_ALLOCATOR( ShaderParamFloat, AM_DEFAULT );

private:
   IDString             m_name;
   float                m_val;

public:
   ShaderParamFloat( const IDString& nameId, float val );

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
   DECLARE_ALLOCATOR( ShaderParamFloatArray, AM_DEFAULT );

private:
   IDString             m_name;
   float*               m_val;
   unsigned int         m_size;

public:
   ShaderParamFloatArray( const IDString& nameId, const float* arr, unsigned int size );
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
   DECLARE_ALLOCATOR( ShaderParamMtx, AM_ALIGNED_16 );

private:
   IDString             m_name;
   Matrix               m_val;

public:
   ShaderParamMtx( const IDString& nameId, const Matrix& val );

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
   DECLARE_ALLOCATOR( ShaderParamMtxArray, AM_DEFAULT );

private:
   IDString             m_name;
   Matrix*              m_val;
   unsigned int         m_size;

public:
   ShaderParamMtxArray( const IDString& nameId, const Matrix* arr, unsigned int size );
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
   DECLARE_ALLOCATOR( ShaderParamVec4, AM_ALIGNED_16 );

private:
   IDString             m_name;
   Vector               m_val;

public:
   ShaderParamVec4( const IDString& nameId, const Vector& val );

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
   DECLARE_ALLOCATOR( ShaderParamVec4Array, AM_DEFAULT );

private:
   IDString             m_name;
   Vector*              m_val;
   unsigned int         m_size;

public:
   ShaderParamVec4Array( const IDString& nameId, const Vector* arr, unsigned int size );
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
   DECLARE_ALLOCATOR( ShaderParamString, AM_DEFAULT );

private:
   IDString             m_name;
   std::string          m_val;

public:
   ShaderParamString( const IDString& nameId, const std::string& val );

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
   DECLARE_ALLOCATOR( ShaderParamTexture, AM_DEFAULT );

private:
   IDString             m_name;
   Texture*             m_val;

public:
   ShaderParamTexture( const IDString& nameId, Texture* val );

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
   DECLARE_ALLOCATOR( ShaderParamRenderTarget, AM_DEFAULT );

private:
   IDString             m_name;
   RenderTarget&        m_val;

public:
   ShaderParamRenderTarget( const IDString& nameId, RenderTarget& val );

   // -------------------------------------------------------------------------
   // ShaderParam implementation
   // -------------------------------------------------------------------------
   void setParam( Renderer& renderer, void* shaderPtr );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\ShaderParam.inl"

///////////////////////////////////////////////////////////////////////////////

#endif _SHADER_PARAM_H
