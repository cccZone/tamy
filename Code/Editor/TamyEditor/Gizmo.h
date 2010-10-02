#pragma once

/// @file   TamyEditor\PositionGizmo.h
/// @brief  A selected object manipulation gizmo.

#include "core-Renderer\SpatialEntity.h"
#include "core-Renderer\Renderable.h"


///////////////////////////////////////////////////////////////////////////////

class LineSegments;
class GizmoEffect;
class Camera;
class ResourcesManager;
class Geometry;

///////////////////////////////////////////////////////////////////////////////

/**
 * A gizmo showing the selected object's orientation and indicating 
 * the selected manipulation mode ( moving, rotating, scaling ).
 */
class Gizmo : public SpatialEntity, public Renderable
{
public:
   enum Mode
   {
      GM_TRANSLATION,
      GM_ROTATION,
      GM_SCALING
   };

private:
   SpatialEntity& m_node;
   Mode           m_mode;

   LineSegments*  m_translationAxes;
   LineSegments*  m_rotationAxes;
   LineSegments*  m_scalingAxes;

   GizmoEffect*   m_effect;
   LineSegments*  m_geometry;

   Attributes     m_attributes;

public:
   /**
    * Constructor.
    */
   Gizmo( SpatialEntity& node, ResourcesManager& rm, Camera& camera );
   ~Gizmo();

   /**
    * Sets a new mode in which the gizmo will be displayed.
    */
   void setMode( Mode mode );

   // -------------------------------------------------------------------------
   // Renderable representation
   // -------------------------------------------------------------------------
   void render();
   const Attributes& getAttributes() const { return m_attributes; }
};

///////////////////////////////////////////////////////////////////////////////
