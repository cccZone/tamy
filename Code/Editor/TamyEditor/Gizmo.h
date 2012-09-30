/// @file   TamyEditor\Gizmo.h
/// @brief  A selected object manipulation gizmo.
#pragma once

#include "core\Color.h"
#include "core-MVC\SpatialEntity.h"
#include "core-Renderer\RenderState.h"
#include "core-AppFlow\TimeDependent.h"


///////////////////////////////////////////////////////////////////////////////

class SpatialEntity;
class Renderer;
class GizmoAxis;
class GizmoMaterial;
class TriangleMesh;
class TranslationGizmoOp;
class RotationGizmoOp;
class ScalingGizmoOp;
class Camera;

///////////////////////////////////////////////////////////////////////////////

/**
 * A gizmo showing the selected object's orientation and indicating 
 * the selected manipulation mode ( moving, rotating, scaling ).
 */
class Gizmo : public SpatialEntity, public TimeDependent
{
   DECLARE_CLASS()

public:
   enum Mode
   {
      GM_TRANSLATION,
      GM_ROTATION,
      GM_SCALING
   };

private:
   const Color             OX_COLOR;
   const Color             OY_COLOR;
   const Color             OZ_COLOR;

   SpatialEntity&          m_node;
   Mode                    m_mode;

   TriangleMesh*           m_meshes[3];
   GizmoMaterial*          m_axisMaterial[3];
   GizmoAxis*              m_geometry[3];

   // operations
   TranslationGizmoOp*     m_translationOp;
   RotationGizmoOp*        m_rotationOp;
   ScalingGizmoOp*         m_scalingOp;

public:
   /**
    * Constructor.
    *
    * @param node             node for which the gizmo will be drawn
    * @param mode             mode in which the gizmo should work
    * @param activeCamera
    */
   Gizmo( SpatialEntity& node, Mode mode, Camera& activeCamera );
   ~Gizmo();

   /**
    * Sets a new mode in which the gizmo will be displayed.
    */
   void setMode( Mode mode );

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );

private:
   void createTranslationGizmo();
   void createRotationGizmo();
   void createScalingGizmo();
};

///////////////////////////////////////////////////////////////////////////////
