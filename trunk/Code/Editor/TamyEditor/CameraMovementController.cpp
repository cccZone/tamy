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
   float speedMul = m_uic->isKeyPressed( VK_SHIFT ) ? 4.0f : 1.0f;
   float movementSpeed = 10.0f * speedMul * timeElapsed;
   float rotationSpeed = 0.1f * timeElapsed;

   // check which keys are pressed
   m_movementDir[MD_FRONT] = m_uic->isKeyPressed( 'W' );
   m_movementDir[MD_BACK] = m_uic->isKeyPressed( 'S' );
   m_movementDir[MD_LEFT] = m_uic->isKeyPressed( 'A' );
   m_movementDir[MD_RIGHT] = m_uic->isKeyPressed( 'D' );

   m_rotating = m_uic->isKeyPressed( VK_RBUTTON );
   m_uic->setRelativeMouseMovement( m_rotating );

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
