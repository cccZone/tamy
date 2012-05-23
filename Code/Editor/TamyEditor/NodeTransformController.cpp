#include "NodeTransformController.h"
#include "TamySceneWidget.h"
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

NodeTransformController::NodeTransformController( const std::vector< SpatialEntity* >& nodes, NodeTransformControlMode mode )
   : m_uic( NULL )
   , m_renderer( NULL )
   , m_camera( NULL )
   , m_nodes( nodes )
   , m_controlMode( mode )
   , m_rotationAxis( GLOBAL_AXES[0] )
{
   memset( m_movementDir, false, sizeof( bool ) * 4 );
}

///////////////////////////////////////////////////////////////////////////////

NodeTransformController::~NodeTransformController()
{
   m_nodes.clear();
}

///////////////////////////////////////////////////////////////////////////////

void NodeTransformController::initialize( TamySceneWidget& widget )
{
   m_uic = &widget;
   m_renderer = &widget.getRenderer();
   m_camera = &m_renderer->getActiveCamera();

   m_uic->setRelativeMouseMovement( false );

   memset( m_movementDir, false, sizeof( bool ) * 4 );
   selectGlobalRotationAxis( m_rotationAxis );
}

///////////////////////////////////////////////////////////////////////////////

void NodeTransformController::update( float timeElapsed )
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
      valChange.y = m_uic->getMouseSpeed().v[1] * movementSpeed;
   }

   // transform the manipulated nodes
   unsigned int count = m_nodes.size();
   for( unsigned int i = 0; i < count; ++i )
   {
      transformNode( *m_nodes[i], valChange );
   }
}

///////////////////////////////////////////////////////////////////////////////

void NodeTransformController::transformNode( Node& node, const Vector& valChange ) const
{
   Matrix& localMtx = node.accessLocalMtx();
   switch( m_controlMode )
   {
   case NTM_TRANSLATE:
      {
         // calculate the camera relative translation
         Vector vecX, vecY, vecZ, camPos;
         m_camera->getGlobalVectors( vecX, vecY, vecZ, camPos );

         vecX.normalize();
         vecY.normalize();

         Vector camRelValsX, camRelValsY, camRelVals;
         camRelValsX.setMul( vecX, valChange.x );
         camRelValsY.setMul( vecY, valChange.y );
         camRelVals.setSub( camRelValsX, camRelValsY );

         // manipulate the node
         Matrix changeMtx;
         changeMtx.setTranslation( camRelVals );
         localMtx.preMul( changeMtx );
         break;
      }

   case NTM_ROTATE:
      {
         // manipulate the node
         Quaternion rotQ;
         rotQ.setAxisAngle( m_rotationAxis, valChange.x + valChange.y );
         Matrix changeMtx;
         changeMtx.setRotation( rotQ );
         localMtx.preMul( changeMtx );
         break;
      }

   default:
      {
         break;
      }
   }

   // regenerate node's transformation matrix' vectors to ensure their orthogonality
   Vector rightVec, upVec, lookVec;
   node.getRightVec( rightVec );
   node.getUpVec( upVec );
   node.getLookVec( lookVec );

   lookVec.normalize();
   rightVec.setCross( upVec, lookVec );
   rightVec.normalize();
   upVec.setCross( lookVec, rightVec );
   upVec.normalize();

   node.setRightVec( rightVec );
   node.setUpVec( upVec );
   node.setLookVec( lookVec );
}

///////////////////////////////////////////////////////////////////////////////

void NodeTransformController::selectGlobalRotationAxis( Vector& outRotationAxis ) const
{
   /*
   int axisIdx = 0;
   if ( !m_nodes.empty() )
   {
      // get the direction vector from the camera to the object
      D3DXVECTOR3 objPos, camPos;
      D3DXVECTOR3 objAxes[3];
      D3DXVECTOR3 camAxes[3];
      m_camera->getGlobalVectors( camAxes[0], camAxes[1], camAxes[2], camPos );
      m_nodes[0]->getGlobalVectors( objAxes[0], objAxes[1], objAxes[2], objPos );
      D3DXVECTOR3 dirToObj = objPos - camPos;
      D3DXVec3Normalize( &dirToObj, &dirToObj );

      // find the axis we're closest to and select it for rotation
      float smallestAngle = 181.f;
      for ( int i = 0; i < 3; ++i )
      {
         float angle = 1.f - fabs( D3DXVec3Dot( &dirToObj, &objAxes[i] ) );
         if ( angle < smallestAngle )
         {
            smallestAngle = angle;
            axisIdx = i;
         }
      }
      ASSERT_MSG( axisIdx >= 0, "Rotation axis not found" );
   }

   outRotationAxis = GLOBAL_AXES[ axisIdx ];*/

   // always rotate around OY axis
   outRotationAxis = GLOBAL_AXES[ 1 ];
}

///////////////////////////////////////////////////////////////////////////////

void NodeTransformController::keySmashed( unsigned char keyCode )
{

}

///////////////////////////////////////////////////////////////////////////////

void NodeTransformController::keyHeld( unsigned char keyCode )
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

void NodeTransformController::keyReleased( unsigned char keyCode )
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
