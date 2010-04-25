#include "core-AppFlow\UserInputController.h"


///////////////////////////////////////////////////////////////////////////////

UserInputController::UserInputController()
      : m_relativePt(512, 512),
      m_mouseSpeed(0, 0),
      m_relativeModeOn(false)
{
   ZeroMemory(m_keyBuffer, 256 * sizeof(unsigned char));
}

///////////////////////////////////////////////////////////////////////////////

void UserInputController::setRelativeMouseMovement(bool enable)
{
   m_relativeModeOn = enable;
   if (enable)
   {
      setMousePos(m_relativePt);
      onRelativeMouseMovement();
   }
   else
   {
      onAbsoluteMouseMovement();
   }
}

///////////////////////////////////////////////////////////////////////////////

void UserInputController::update(float timeElapsed)
{
   Point newMousePos;
   checkUserInput(m_keyBuffer, newMousePos);

   Point mousePosChange;
   if (m_relativeModeOn)
   {
      setMousePos(m_relativePt);
      mousePosChange = newMousePos - m_relativePt;
   }
   else
   {
      mousePosChange = newMousePos - m_mousePos;
   }

   // determine mouse speed
   if (timeElapsed > 0)
   {
      m_mouseSpeed.x = (float)(mousePosChange.x) / timeElapsed;
      m_mouseSpeed.y = (float)(mousePosChange.y) / timeElapsed;
   }
   else
   {
      m_mouseSpeed.x = 0;
      m_mouseSpeed.y = 0;
   }

   // set the mouse and cursor position
   m_mousePos = m_mousePos + mousePosChange;
}

///////////////////////////////////////////////////////////////////////////////

bool UserInputController::isKeyPressed(unsigned char keyCode) const
{
   return (m_keyBuffer[keyCode] & 0xF0) != 0; 
}

///////////////////////////////////////////////////////////////////////////////

void UserInputController::setKey(unsigned char* buf, 
                                 unsigned char keyCode, 
                                 bool pressed)
{
   if (pressed)
   {
      buf[keyCode] = 0xF0; 
   }
   else
   {
      buf[keyCode] = 0; 
   }
}

///////////////////////////////////////////////////////////////////////////////
