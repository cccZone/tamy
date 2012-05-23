#include "CameraMovementController.h"
#include "TamySceneWidget.h"
#include "core-Renderer/Camera.h"
#include "ext-MotionControllers/UnconstrainedMotionController.h"


///////////////////////////////////////////////////////////////////////////////

CameraMovementController::CameraMovementController()
   : m_uic( NULL )
   , m_renderer( NULL )
   , m_cameraController( NULL )
   , m_rotating( false )
{
   memset( m_movementDir, false, sizeof( bool ) * 4 );
}

///////////////////////////////////////////////////////////////////////////////

CameraMovementController::~CameraMovementController()
{
   if ( m_uic )
   {
      m_uic->setRelativeMouseMovement( false );
   }

   delete m_cameraController;
   m_cameraController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void CameraMovementController::initialize( TamySceneWidget& widget )
{
   Camera& camera = widget.getCamera();
   m_uic = &widget;
   m_renderer = &widget.getRenderer();

   m_cameraController = new UnconstrainedMotionController( camera );

   m_rotating = false;
   memset( m_movementDir, false, sizeof( bool ) * 4 );
}

///////////////////////////////////////////////////////////////////////////////

void CameraMovementController::update( float timeElapsed )
{
   float movementSpeed = 40 * timeElapsed;
   float rotationSpeed = 0.1f * timeElapsed;

   // process the keys
   Vector moveVec;
   if ( m_movementDir[MD_FRONT] )  
   { 
      moveVec.setMul( m_cameraController->getLookVec(), movementSpeed );
      m_cameraController->move( moveVec ); 
   }

   if ( m_movementDir[MD_BACK] )
   { 
      moveVec.setMul( m_cameraController->getLookVec(), -movementSpeed );
      m_cameraController->move( moveVec ); 
   }

   if ( m_movementDir[MD_LEFT] )   
   { 
      moveVec.setMul( m_cameraController->getRightVec(), -movementSpeed );
      m_cameraController->move( moveVec ); 
   }

   if ( m_movementDir[MD_RIGHT] )
   { 
      moveVec.setMul( m_cameraController->getRightVec(), movementSpeed );
      m_cameraController->move( moveVec ); 
   }

   if ( m_rotating )
   {
      float mouseSpeedX = m_uic->getMouseSpeed().v[0] * rotationSpeed;
      float mouseSpeedY = m_uic->getMouseSpeed().v[1] * rotationSpeed;
      m_cameraController->rotate( mouseSpeedY, mouseSpeedX );
   }
}

///////////////////////////////////////////////////////////////////////////////

void CameraMovementController::keySmashed( unsigned char keyCode )
{
   // this controller doesn't react to sudden key smashes
}

///////////////////////////////////////////////////////////////////////////////

void CameraMovementController::keyHeld( unsigned char keyCode )
{
   switch( keyCode )
   {
   case VK_RBUTTON:
      {
         m_rotating = true;
         m_uic->setRelativeMouseMovement( m_rotating );
         break;
      }

   case 'W':
      {
         m_movementDir[MD_FRONT] = true;
         break;
      }

   case 'S':
      {
         m_movementDir[MD_BACK] = true;
         break;
      }

   case 'A':
      {
         m_movementDir[MD_LEFT] = true;
         break;
      }

   case 'D':
      {
         m_movementDir[MD_RIGHT] = true;
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void CameraMovementController::keyReleased( unsigned char keyCode )
{
   switch( keyCode )
   {
   case VK_RBUTTON:
      {
         m_rotating = false;
         m_uic->setRelativeMouseMovement( m_rotating );
         break;
      }

   case 'W':
      {
         m_movementDir[MD_FRONT] = false;
         break;
      }

   case 'S':
      {
         m_movementDir[MD_BACK] = false;
         break;
      }

   case 'A':
      {
         m_movementDir[MD_LEFT] = false;
         break;
      }

   case 'D':
      {
         m_movementDir[MD_RIGHT] = false;
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
