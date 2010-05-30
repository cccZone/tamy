#pragma once

/// @file   TamyEditor\PositionGizmo.h
/// @brief  A selected object manipulation gizmo.

#include "core-Renderer\SpatialEntity.h"


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
class Gizmo : public SpatialEntity
{
private:
   enum Mode
   {
      GM_TRANSLATION,
      GM_ROTATION,
      GM_SCALING
   };

private:
   Mode           m_mode;

   LineSegments*  m_translationAxes;
   LineSegments*  m_rotationAxes;
   LineSegments*  m_scalingAxes;

   GizmoEffect*   m_effect;
   Geometry*      m_geometry;

public:
   /**
    * Constructor.
    */
   Gizmo( ResourcesManager& rm, Camera& camera );
   ~Gizmo();

   /**
    * Sets a new mode in which the gizmo will be displayed.
    */
   void setMode( Mode mode );

   /**
    * Renders the gizmo on screen.
    *
    * @param objectGlobalMtx     global matrix of an object which the gizmo
    *                            represents
    */
   void render( const D3DXMATRIX& objectGlobalMtx );
};

///////////////////////////////////////////////////////////////////////////////
