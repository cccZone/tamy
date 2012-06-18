/// @file   TamyEditor/SingleColorDebugMat.h
/// @brief  a material that draws things with a single color
#pragma once

#include "core/ReflectionObject.h"
#include "core-Renderer/RenderState.h"
#include "core/Color.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;

///////////////////////////////////////////////////////////////////////////////

class SingleColorDebugMat : public ReflectionObject, public TRenderState< SingleColorDebugMat >
{
   DECLARE_CLASS();

private:
   Color                   m_color;
   PixelShader*            m_shader;

public:
   /**
    * Constructor.
    *
    * @param axisIdx
    * @param color
    * @param shader
    */
   SingleColorDebugMat( const Color& color );

   // -------------------------------------------------------------------------
   // TRenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer ) const;
   void onPostRender( Renderer& renderer ) const;
   bool onEquals( const SingleColorDebugMat& rhs ) const { return m_color == rhs.m_color; }
   bool onLess( const SingleColorDebugMat& rhs ) const { return m_color < rhs.m_color; }
};

///////////////////////////////////////////////////////////////////////////////
