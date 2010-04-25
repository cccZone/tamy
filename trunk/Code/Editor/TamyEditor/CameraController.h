#pragma once

/// @file   TamyEditor\CameraController.h
/// @brief  controller of scene camera movements

#include "core-AppFlow.h"
#include <map>


///////////////////////////////////////////////////////////////////////////////

class CTimer;
class UnconstrainedMotionController;
class UserInputController;
class CameraCommand;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

class CameraController : public KeyStatusHandler
{
private:
   UnconstrainedMotionController& m_cameraController;
   UserInputController& m_uiController;
   Renderer& m_renderer;

   bool m_rotating;
   bool m_forward;
   bool m_backward;
   bool m_left;
   bool m_right;

   typedef std::map<unsigned char, CameraCommand*> CommandsMap;
   CommandsMap m_buttonPressedCommands;

public:
   /**
    * Constructor.
    *
    * @param cameraController    camera movement controller
    * @param uiController        user input receiver
    * @param renderer            active renderer
    */
   CameraController(UnconstrainedMotionController& cameraController,
                    UserInputController& uiController,
                    Renderer& renderer);
   ~CameraController();

   /**
    * The method updates the status of the controller.
    *
    * @param timeElapsed   amount of time elapsed since the last time this
    *                      method was called (in seconds)
    */
   void update(float timeElapsed);

   // -------------------------------------------------------------------------
   // Commands management
   // -------------------------------------------------------------------------
   /**
    * Adds a new command that will be executed when a key is pressed.
    *
    * @param keyCode    key which when pressed will toggle the command execution
    * @param command    command to execute
    */
   void addButtonPressCommand(unsigned char keyCode, CameraCommand* command);

   // -------------------------------------------------------------------------
   // KeyStatusHandler implementation
   // -------------------------------------------------------------------------
   void keySmashed(unsigned char keyCode);
   void keyHeld(unsigned char keyCode);
   void keyReleased(unsigned char keyCode);
};

///////////////////////////////////////////////////////////////////////////////
