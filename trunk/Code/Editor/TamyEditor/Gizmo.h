#pragma once

/// @file   TamyEditor\Gizmo.h
/// @brief  A selected object manipulation gizmo.

#include "core\Color.h"
#include "core\IDebugDraw.h"


///////////////////////////////////////////////////////////////////////////////

class SpatialEntity;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A gizmo showing the selected object's orientation and indicating 
 * the selected manipulation mode ( moving, rotating, scaling ).
 */
class Gizmo : public IDebugDrawable
{
public:
   enum Mode
   {
      GM_TRANSLATION,
      GM_ROTATION,
      GM_SCALING
   };

private:
   const float       SIZE;
   const Color       OX_COLOR;
   const Color       OY_COLOR;
   const Color       OZ_COLOR;

   SpatialEntity& m_node;
   Mode           m_mode;

public:
   /**
    * Constructor.
    *
    * @param node       node for which the gizmo will be drawn
    */
   Gizmo( SpatialEntity& node );
   ~Gizmo();

   /**
    * Sets a new mode in which the gizmo will be displayed.
    */
   void setMode( Mode mode );

   // -------------------------------------------------------------------------
   // IDebugDrawable implementation
   // -------------------------------------------------------------------------
   void onDebugRender( Renderer& renderer ) const;

private:
   void drawTranslationGizmo( Renderer& renderer ) const;
   void drawRotationGizmo( Renderer& renderer ) const;
   void drawScalingGizmo( Renderer& renderer ) const;
};

///////////////////////////////////////////////////////////////////////////////
