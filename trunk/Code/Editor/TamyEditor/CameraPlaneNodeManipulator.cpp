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
      Vector( Quad_1000 ),
      Vector( Quad_0100 ),
      Vector( Quad_0010 )
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
      if ( m_movementDir[MD_FRONT] )  { valChange[1] += movementSpeed; }
      if ( m_movementDir[MD_BACK] )   { valChange[1] -= movementSpeed; }
      if ( m_movementDir[MD_LEFT] )   { valChange[0] -= movementSpeed; }
      if ( m_movementDir[MD_RIGHT] )  { valChange[0] += movementSpeed; }
   }
   else
   {
      valChange[0] = m_sceneWidget->getMouseSpeed().v[0] * movementSpeed;

      // we need to negate this value, 'cause we want it expressed in viewport space ( where Y axis goes up, not down )
      valChange[1] = -m_sceneWidget->getMouseSpeed().v[1] * movementSpeed;
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
   sideDisplacement.setMul( cameraGlobalMtx.sideVec(), valChange.getComponent(0) );
   upDisplacement.setMul( cameraGlobalMtx.upVec(), valChange.getComponent(1) );

   Vector displacement;
   displacement.setAdd( sideDisplacement, upDisplacement );

 
   // apply the transform
   Quaternion parentOrientation;
   Vector transformedDisplacement;
   Matrix transformMtx;

   uint count = m_selectedNodes.size();
   for ( uint i = 0; i < count; ++i )
   {
      SpatialEntity* entity = m_selectedNodes[i];
      Node* parentNode = entity->getParentNode();
      if ( parentNode )
      {
         // if it's a child node that we're moving, move it with respect to it's parent node coordinate system
         parentNode->getGlobalMtx().getRotation( parentOrientation );
         parentOrientation.conjugate();
         parentOrientation.transform( displacement, transformedDisplacement );

         transformMtx.setTranslation( transformedDisplacement );
      }
      else
      {
         transformMtx.setTranslation( displacement );
      }

      m_selectedNodes[i]->accessLocalMtx().mul( transformMtx );
   }
}

///////////////////////////////////////////////////////////////////////////////

void CameraPlaneNodeManipulator::rotateSelectedNodes( const Vector& valChange ) const
{
   const Matrix& cameraGlobalMtx = m_camera->getLocalMtx();

   const FastFloat valChange0 = valChange.getComponent(0);
   const FastFloat valChange1 = valChange.getComponent(1);
   FastFloat valChange0Sq;
   valChange0Sq.setMul( valChange0, valChange0 );
   FastFloat valChange1Sq;
   valChange1Sq.setMul( valChange1, valChange1 );

   FastFloat rollAngleMag;
   rollAngleMag.setAdd( valChange0Sq, valChange1Sq);
   rollAngleMag.sqrt();

   FastFloat rollAngleSign;
   rollAngleSign.setAdd( valChange0, valChange1 );
   rollAngleSign.neg();

   FastFloat rollAngle;
   rollAngle.setFlipSign( rollAngleMag, rollAngleSign );
   rollAngle.mul( Float_Deg2Rad );

   Quaternion rollQuat;
   rollQuat.setAxisAngle( cameraGlobalMtx.forwardVec(), rollAngle );

   // apply the transform
   Transform currentTransform;
   Quaternion parentOrientation, entityOrientation, newEntityOrientation;
   Matrix transformMtx;
   uint count = m_selectedNodes.size();
   for ( uint i = 0; i < count; ++i )
   {
      SpatialEntity* entity = m_selectedNodes[i];

      const Matrix& entityLocalMtx = entity->getLocalMtx();
      currentTransform.set( entityLocalMtx );

      Node* parentNode = entity->getParentNode();
      if ( parentNode )
      {
         // if it's a child node that we're moving, move it with respect to it's parent node coordinate system
         // @see Quaternion::worldToLocalTransform test to see exactly how and why this works

         entity->getGlobalMtx().getRotation( entityOrientation );
         newEntityOrientation.setMul( entityOrientation, rollQuat );

         parentNode->getGlobalMtx().getRotation( parentOrientation );
         parentOrientation.conjugate();

         currentTransform.m_rotation.setMul( newEntityOrientation, parentOrientation );
      }
      else
      {
         currentTransform.m_rotation.mul( rollQuat );
      }

      currentTransform.toMatrix( transformMtx );
      entity->setLocalMtx( transformMtx );
   }
}

///////////////////////////////////////////////////////////////////////////////
