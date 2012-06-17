/// @file   TamyEditor/GizmoMaterial.h
/// @brief  material gizmo is drawn with
#pragma once

#include "core/ReflectionObject.h"
#include "core-Renderer/RenderState.h"
#include "core/Color.h"


///////////////////////////////////////////////////////////////////////////////

class GizmoMaterial : public ReflectionObject, public TRenderState< GizmoMaterial >
{
   DECLARE_CLASS();

private:
   byte        m_axisIdx;
   Color       m_color;

public:
   /**
    * Constructor.
    *
    * @param axisIdx
    * @param color
    */
   GizmoMaterial( byte axisIdx, const Color& color );

   // -------------------------------------------------------------------------
   // TRenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer ) const;
   void onPostRender( Renderer& renderer ) const;
   bool onEquals( const GizmoMaterial& rhs ) const { return m_axisIdx == rhs.m_axisIdx; }
   bool onLess( const GizmoMaterial& rhs ) const { return m_axisIdx < rhs.m_axisIdx; }
};

///////////////////////////////////////////////////////////////////////////////
