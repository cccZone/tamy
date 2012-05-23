#include "core-AppFlow\UserInputController.h"


///////////////////////////////////////////////////////////////////////////////

UserInputController::UserInputController()
: m_relativePt( 0, 0 )
, m_relativeModeOn(0)
{
   memset(m_keyBuffer, 256 * sizeof(unsigned char), 0);

   m_mouseSpeed.setUniform( 0 );
}

///////////////////////////////////////////////////////////////////////////////

void UserInputController::setRelativeMouseMovement( bool enable )
{
   if ( !m_relativeModeOn && enable )
   {
      // the relative mouse movement was just enabled - memorize the cursor position
      m_relativePt = m_mousePos;
   }

   m_relativeModeOn = enable;

   if ( m_relativeModeOn )
   {
      setMousePos( m_relativePt );
      onRelativeMouseMovement();
   }
   else
   {
      onAbsoluteMouseMovement();
   }
}

///////////////////////////////////////////////////////////////////////////////

void UserInputController::update( float timeElapsed )
{
   Point newMousePos;
   checkUserInput( m_keyBuffer, newMousePos );

   Point mousePosChange;
   if ( isRelativeMouseMovementOn() )
   {
      setMousePos( m_relativePt );
      mousePosChange = newMousePos - m_relativePt;
   }
   else
   {
      mousePosChange = newMousePos - m_mousePos;
   }
   

   // determine mouse speed
   if ( timeElapsed > 0 )
   {
      m_mouseSpeed.v[0] = ( (float)mousePosChange.x ) / timeElapsed;
      m_mouseSpeed.v[1] = ( (float)mousePosChange.y ) / timeElapsed;
   }
   else
   {
      m_mouseSpeed.v[0] = 0;
      m_mouseSpeed.v[1] = 0;
   }

   // set the mouse and cursor position
   m_mousePos = newMousePos;
}

///////////////////////////////////////////////////////////////////////////////

bool UserInputController::isKeyPressed(unsigned char keyCode) const
{
   return (m_keyBuffer[keyCode] & 0xF0) != 0; 
}

///////////////////////////////////////////////////////////////////////////////

void UserInputController::setKey( unsigned char* buf, unsigned char keyCode, bool pressed )
{
   if ( pressed )
   {
      buf[keyCode] = 0xF0; 
   }
   else
   {
      buf[keyCode] = 0; 
   }
}

///////////////////////////////////////////////////////////////////////////////
