#pragma once

/// @file   TamyEditor\SceneNavigator.h
/// @brief  scene navigation component

#include "core\Component.h"


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class UnconstrainedMotionController;
class TimeController;
class Camera;
class UserInputController;
class Renderer;
class KeysStatusManager;
class SceneQueries;
class SelectionManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * This component is responsible for navigating scene.
 */
class SceneNavigator : public Component<TamyEditor>
{
private:
   // required services
   Camera*                          m_camera;
   UserInputController*             m_uic;
   Renderer*                        m_renderer;
   KeysStatusManager*               m_ksm; 
   SceneQueries*                  m_scene;
   TimeController*                  m_timeController;
   SelectionManager*                m_selectionMgr;

   // instance data
   TamyEditor*                      m_mgr;
   UnconstrainedMotionController*   m_cameraController;

public:
   /**
    * Constructor.
    */
   SceneNavigator();
   ~SceneNavigator();

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );
   void onServiceRegistered( TamyEditor& mgr );

private:
   void reset();
};

///////////////////////////////////////////////////////////////////////////////
