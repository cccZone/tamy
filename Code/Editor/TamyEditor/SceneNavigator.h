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

enum MovementDirection
{
   MD_LEFT,
   MD_RIGHT,
   MD_FRONT,
   MD_BACK
};

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
   SceneQueries*                    m_scene;
   TimeController*                  m_timeController;
   SelectionManager*                m_selectionMgr;

   // camera movement state
   bool                             m_rotating;
   bool                             m_movementDir[4];

   // instance data
   TamyEditor*                      m_mgr;
   UnconstrainedMotionController*   m_cameraController;

public:
   /**
    * Constructor.
    */
   SceneNavigator();
   ~SceneNavigator();

   /**
    * Updates the position of the camera.
    */
   void update( float timeElapsed ); 

   // -------------------------------------------------------------------------
   // Camera movement commands
   // -------------------------------------------------------------------------

   /**
    * Sets or resets camera rotation mode direction.
    *
    * @param engage     'true' will make camera rotate, 
    *                   'false' will stop it
    */
   void setCameraRotation( bool engage );

   /**
    * Sets or resets camera movement in the specified direction.
    *
    * @param direction
    * @param engage     if 'true', camera will start moving in the specified
    *                   direction, 'false' will stop it
    */
   void setCameraMove( MovementDirection direction, bool engage );

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );
   void onServiceRegistered( TamyEditor& mgr );

private:
   void reset();
};

///////////////////////////////////////////////////////////////////////////////
