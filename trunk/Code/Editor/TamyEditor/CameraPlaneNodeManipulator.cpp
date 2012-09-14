#include "CameraPlaneNodeManipulator.h"
#include "TamySceneWidget.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/Camera.h"
#include "core-MVC/SpatialEntity.h"
#include "core/Matrix.h"
#include "core/Frustum.h"
#include "core/Plane.h"


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

CameraPlaneNodeManipulator::CameraPlaneNodeManipulator( const std::vector< SpatialEntity* >& selectedNodes )
   : m_sceneWidget( NULL )
   , m_renderer( NULL )
   , m_camera( NULL )
   , m_selectedNodes( selectedNodes )
{
   memset( m_movementDir, false, sizeof( bool ) * 4 );
}

///////////////////////////////////////////////////////////////////////////////

CameraPlaneNodeManipulator::~CameraPlaneNodeManipulator()
{
}

///////////////////////////////////////////////////////////////////////////////

void CameraPlaneNodeManipulator::initialize( TamySceneWidget& widget )
{
   m_sceneWidget = &widget;
   m_renderer = &widget.getRenderer();
   m_camera = &m_renderer->getActiveCamera();

   m_sceneWidget->setRelativeMouseMovement( false );

   memset( m_movementDir, false, sizeof( bool ) * 4 );
}

///////////////////////////////////////////////////////////////////////////////

void CameraPlaneNodeManipulator::update( float timeElapsed )
{
   // check which keys are pressed
   m_movementDir[MD_FRONT] = m_sceneWidget->isKeyPressed( 'W' );
   m_movementDir[MD_BACK] = m_sceneWidget->isKeyPressed( 'S' );
   m_movementDir[MD_LEFT] = m_sceneWidget->isKeyPressed( 'A' );
   m_movementDir[MD_RIGHT] = m_sceneWidget->isKeyPressed( 'D' );

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
      valChange.x = m_sceneWidget->getMouseSpeed().v[0] * movementSpeed;

      // we need to negate this value, 'cause we want it expressed in viewport space ( where Y axis goes up, not down )
      valChange.y = -m_sceneWidget->getMouseSpeed().v[1] * movementSpeed;
   }

   // manipulate the selected nodes
   switch( m_sceneWidget->getGizmoMode() )
   {
   case Gizmo::GM_TRANSLATION:
      {
         translateSelectedNodes( valChange );
         break;
      }

   case Gizmo::GM_ROTATION:
      {
         rotateSelectedNodes( valChange );
         break;
      }

   }
}

///////////////////////////////////////////////////////////////////////////////

void CameraPlaneNodeManipulator::translateSelectedNodes( const Vector& valChange ) const
{
   const Matrix& cameraGlobalMtx = m_camera->getGlobalMtx();
   
   Vector sideDisplacement, upDisplacement;
   sideDisplacement.setMul( cameraGlobalMtx.sideVec(), valChange.x );
   upDisplacement.setMul( cameraGlobalMtx.upVec(), valChange.y );

   Vector displacement;
   displacement.setAdd( sideDisplacement, upDisplacement );

   Matrix transformMtx;
   transformMtx.setTranslation( displacement );

   // apply the transform
   uint count = m_selectedNodes.size();
   for ( uint i = 0; i < count; ++i )
   {
      m_selectedNodes[i]->accessLocalMtx().mul( transformMtx );
   }
}

///////////////////////////////////////////////////////////////////////////////

void CameraPlaneNodeManipulator::rotateSelectedNodes( const Vector& valChange ) const
{
   const Matrix& cameraGlobalMtx = m_camera->getLocalMtx();

   float rollAngleMag = sqrt( valChange.x * valChange.x + valChange.y * valChange.y );
   float rollAngleSign = ( valChange.x + valChange.y ) >= 0 ? 1.0f : -1.0f;
   float rollAngle = DEG2RAD( rollAngleMag * rollAngleSign );

   Quaternion rollQuat;
   rollQuat.setAxisAngle( cameraGlobalMtx.forwardVec(), rollAngle );

   // apply the transform
   Transform currentTransform;

   Matrix transformMtx;
   SpatialEntity* entity;
   uint count = m_selectedNodes.size();
   for ( uint i = 0; i < count; ++i )
   {
      entity = m_selectedNodes[i];
      const Matrix& entityLocalMtx = entity->getLocalMtx();
      currentTransform.set( entityLocalMtx );

      currentTransform.m_rotation.mul( rollQuat );
      currentTransform.toMatrix( transformMtx );

      // TODO: !!!!!!!! our math works only with left handed systems. Quaternions break when a right handed system matrix
      // is used - so DON'T USE THEM. Export models from Blender using a different convention, instead of matrix shuffling.
      // The exported matrices absolutely need to be left-handed

      entity->setLocalMtx( transformMtx );
   }
}

///////////////////////////////////////////////////////////////////////////////
