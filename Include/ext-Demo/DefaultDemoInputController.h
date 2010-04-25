#pragma once

/// @file   ext-Demo\DefaultDemoInputController.h
/// @brief  an input controller giving you a full controll over the
///         camera and allowing to end a demo with a press of ESC button
            
#include "core\Delegate.h"


///////////////////////////////////////////////////////////////////////////////

class UserInputController;
class UnconstrainedMotionController;
class Camera;

///////////////////////////////////////////////////////////////////////////////

namespace demo
{

///////////////////////////////////////////////////////////////////////////////

typedef Delegate<void ()> EndDemoCommand;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class provides basic control over the demo. It interprets
 * user's input and allows him to steer a predefined camera,
 * as well as it allows to control the lifetime of a demo,
 * ending it as soon as user presses the ESC button.
 */
class DefaultDemoInputController
{
private:
   UserInputController& m_uiController;
   UnconstrainedMotionController* m_cameraController;
   EndDemoCommand m_endDemo;
   bool m_rotating;

public:
   /**
    * Constructor.
    *
    * @param uiController     instance of the user input system we'll query
    *                         for user input events
    * @param camera           camera control over which we give to the user
    * @param endDemoCommand   command that will be invoked when user presses
    *                         the ESC key
    */
   DefaultDemoInputController(UserInputController& uiController,
                              Camera& camera,
                              EndDemoCommand endDemoCommand);

   /**
    * Update the controller state.
    *
    * @param timeElapsed      how much time elapsed since the last time 
    *                         this method was called (in ms)
    */
   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo

///////////////////////////////////////////////////////////////////////////////
