/// @file   TamyEditor/CameraMovementController.h
/// @brief  an input controller that will control camera movements
#pragma once

#include "SceneRendererInputController.h"


///////////////////////////////////////////////////////////////////////////////

class UserInputController;
class Renderer;
class UnconstrainedMotionController;

///////////////////////////////////////////////////////////////////////////////

class CameraMovementController : public SceneRendererInputController
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
   UnconstrainedMotionController*         m_cameraController;

   // camera movement state
   bool                                   m_rotating;
   bool                                   m_movementDir[4];


public:
   CameraMovementController();
   ~CameraMovementController();

   // -------------------------------------------------------------------------
   // SceneRendererInputController implementation
   // -------------------------------------------------------------------------
   void initialize( TamySceneWidget& widget );

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );
};

///////////////////////////////////////////////////////////////////////////////
