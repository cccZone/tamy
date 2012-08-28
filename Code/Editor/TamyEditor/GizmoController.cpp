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
   // check which keys are pressed
   m_movementDir[MD_FRONT] = m_uic->isKeyPressed( 'W' );
   m_movementDir[MD_BACK] = m_uic->isKeyPressed( 'S' );
   m_movementDir[MD_LEFT] = m_uic->isKeyPressed( 'A' );
   m_movementDir[MD_RIGHT] = m_uic->isKeyPressed( 'D' );

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
