/// @file   TamyEditor/SingleColorDebugMat.h
/// @brief  a material that draws things with a single color
#pragma once

#include "core/ReflectionObject.h"
#include "core-Renderer/RenderState.h"
#include "core/Color.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;

///////////////////////////////////////////////////////////////////////////////

class SingleColorDebugMat : public TRenderState< SingleColorDebugMat >
{
   DECLARE_CLASS();

private:
   Color                   m_color;
   PixelShader*            m_shader;
   bool                    m_wireframe;

public:
   /**
    * Constructor.
    *
    * @param color
    * @param wireframe
    */
   SingleColorDebugMat( const Color& color, bool wireframe = false );

   // -------------------------------------------------------------------------
   // TRenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer ) const;
   void onPostRender( Renderer& renderer ) const;
   bool onEquals( const SingleColorDebugMat& rhs ) const { return m_color == rhs.m_color && m_wireframe == rhs.m_wireframe; }
   bool onLess( const SingleColorDebugMat& rhs ) const { return m_color < rhs.m_color && m_wireframe  < rhs.m_wireframe; }
};

///////////////////////////////////////////////////////////////////////////////
