#include "GizmoController.h"
#include "TamySceneWidget.h"
#include "GizmoAxis.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/Camera.h"
#include "core-MVC/SpatialEntity.h"
#include "core/Matrix.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   Vector GLOBAL_AXES[] = {
      Vector::OX,
      Vector::OY,
      Vector::OZ
   };
}

///////////////////////////////////////////////////////////////////////////////

GizmoController::GizmoController( GizmoAxis& gizmoAxis )
   : m_uic( NULL )
   , m_renderer( NULL )
   , m_camera( NULL )
   , m_gizmoAxis( gizmoAxis )
{
   memset( m_movementDir, false, sizeof( bool ) * 4 );
}

///////////////////////////////////////////////////////////////////////////////

GizmoController::~GizmoController()
{
}

///////////////////////////////////////////////////////////////////////////////

void GizmoController::initialize( TamySceneWidget& widget )
{
   m_uic = &widget;
   m_renderer = &widget.getRenderer();
   m_camera = &m_renderer->getActiveCamera();

   m_uic->setRelativeMouseMovement( false );

   memset( m_movementDir, false, sizeof( bool ) * 4 );
}

///////////////////////////////////////////////////////////////////////////////

void GizmoController::update( float timeElapsed )
{
   // calculate the parameter change factor ( relative to the camera orientation )
   float movementSpeed = 0.1f * timeElapsed;
   Vector valChange( 0, 0, 0 );
   bool anyKeyHeld = m_movementDir[MD_FRONT] || m_movementDir[MD_BACK] || m_movementDir[MD_LEFT] || m_movementDir[MD_RIGHT];
   if ( anyKeyHeld )
   {
      if ( m_movementDir[MD_FRONT] )  { valChange.y += movementSpeed; }
      if ( m_movementDir[MD_BACK] )   { valChange.y -= movementSpeed; }
      if ( m_movementDir[MD_LEFT] )   { valChange.x -= movementSpeed; }
      if ( m_movementDir[MD_RIGHT] )  { valChange.x += movementSpeed; }
   }
   else
   {
      valChange.x = m_uic->getMouseSpeed().v[0] * movementSpeed;

      // we need to negate this value, 'cause we want it expressed in viewport space ( where Y axis goes up, not down )
      valChange.y = -m_uic->getMouseSpeed().v[1] * movementSpeed;
   }

   // transform the manipulated nodes
   m_gizmoAxis.transformManipulatedNodes( valChange );
}

///////////////////////////////////////////////////////////////////////////////

void GizmoController::keySmashed( unsigned char keyCode )
{

}

///////////////////////////////////////////////////////////////////////////////

void GizmoController::keyHeld( unsigned char keyCode )
{
   switch( keyCode )
   {
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

void GizmoController::keyReleased( unsigned char keyCode )
{
   switch( keyCode )
   {
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
