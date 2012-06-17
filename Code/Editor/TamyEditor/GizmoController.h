/// @file   TamyEditor/GizmoController.h
/// @brief  manipulates the selected objects through its manipulation gizmo
#pragma once

#include "SceneRendererInputController.h"
#include "core\Vector.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class UserInputController;
class Renderer;
class Camera;
class SpatialEntity;
class Node;
class GizmoAxis;

///////////////////////////////////////////////////////////////////////////////

class GizmoController : public SceneRendererInputController
{
private:
   enum MovementDirection
   {
      MD_LEFT,
      MD_RIGHT,
      MD_FRONT,
      MD_BACK
   };

private:
   UserInputController*                   m_uic;
   const Renderer*                        m_renderer;
   const Camera*                          m_camera;
   GizmoAxis&                             m_gizmoAxis;

   bool                                   m_movementDir[4];

public:
   /**
    * Constructor.
    *
    * @param gizmoAxis
    */
   GizmoController( GizmoAxis& gizmoAxis );
   ~GizmoController();

   // -------------------------------------------------------------------------
   // SceneRendererInputController implementation
   // -------------------------------------------------------------------------
   void initialize( TamySceneWidget& widget );

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );

   // -------------------------------------------------------------------------
   // KeyStatusHandler implementation
   // -------------------------------------------------------------------------
   void keySmashed( unsigned char keyCode );
   void keyHeld( unsigned char keyCode );
   void keyReleased( unsigned char keyCode );
};

///////////////////////////////////////////////////////////////////////////////
