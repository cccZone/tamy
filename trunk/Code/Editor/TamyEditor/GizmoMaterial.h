/// @file   TamyEditor/GizmoMaterial.h
/// @brief  material gizmo is drawn with
#pragma once

#include "core/ReflectionObject.h"
#include "core-Renderer/RenderState.h"
#include "core/Color.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;

///////////////////////////////////////////////////////////////////////////////

class GizmoMaterial : public TRenderState< GizmoMaterial >
{
   DECLARE_CLASS();

private:
   byte                    m_axisIdx;
   Color                   m_color;
   PixelShader&            m_shader;

public:
   /**
    * Constructor.
    *
    * @param axisIdx
    * @param color
    * @param shader
    */
   GizmoMaterial( byte axisIdx, const Color& color, PixelShader& shader );

   // -------------------------------------------------------------------------
   // TRenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer ) const;
   void onPostRender( Renderer& renderer ) const;
   bool onEquals( const GizmoMaterial& rhs ) const { return m_axisIdx == rhs.m_axisIdx; }
   bool onLess( const GizmoMaterial& rhs ) const { return m_axisIdx < rhs.m_axisIdx; }
};

///////////////////////////////////////////////////////////////////////////////
