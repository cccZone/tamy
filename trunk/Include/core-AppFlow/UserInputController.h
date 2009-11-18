#pragma once

/// @file   core-AppFlow\UserInputController.h
/// @brief  user input handling mechanism

#include <d3dx9.h>
#include "core\Point.h"
#include "core-AppFlow\KeysStatusModel.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * User input handling mechanism.
 */
class UserInputController : public KeysStatusModel
{
private:
   Point m_relativePt;
   Point m_prevMousePos;

   unsigned char m_keyBuffer[256];
   Point m_mousePos;
   D3DXVECTOR2 m_mouseSpeed;

   bool m_relativeModeOn;

public:
   UserInputController();
   virtual ~UserInputController() {}

   void update(float timeElapsed);

   /**
    * In relative mouse movement mode the cursor stops moving
    * and becomes obsolete - because what we really care about here
    * is the speed of the mouse movement.
    *
    * In the limited mouse movement mode cursor moves and
    * it's area of movement is bounded by the screen dimensions.
    */
   void setRelativeMouseMovement(bool enable);

   bool isKeyPressed(unsigned char keyCode) const;
   const Point& getMousePos() const {return m_mousePos;}
   const D3DXVECTOR2& getMouseSpeed() const {return m_mouseSpeed;}

protected:
   /**
    * Using this utility method, an implementation can inform about a key
    * being pressed or released
    *
    * @param buf        buffer containing states of keys
    * @param keyCode    code of the changed key
    * @param pressed    'true' if  pressed, 'false' - if released
    */
   void setKey(unsigned char* buf, unsigned char keyCode, bool pressed);

   virtual void onRelativeMouseMovement() = 0;
   virtual void onAbsoluteMouseMovement() = 0;

   /**
    * This method is implementation specific.
    * Upon return both params should contain the current state of
    * the user input:
    *
    * @param keysPressed - keyboard keys as well as mouse keys pressed
    * @param mousePos - absolute position of the mouse
    * @param relMovement - relative position change since the last check
    */
   virtual void checkUserInput(unsigned char* keyBuffer, Point& mousePos) = 0;

   /**
    * This method sets the mouse at the specified position
    */
   virtual void setMousePos(const Point& pos) = 0;
};

///////////////////////////////////////////////////////////////////////////////
