#include "NodeTransformController.h"
#include "TamySceneWidget.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/Camera.h"
#include "core-MVC/SpatialEntity.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   D3DXVECTOR3 GLOBAL_AXES[] = {
      D3DXVECTOR3( 1, 0, 0 ),
      D3DXVECTOR3( 0, 1, 0 ),
      D3DXVECTOR3( 0, 0, 1 )
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
   m_rotationAxis = selectGlobalRotationAxis();
}

///////////////////////////////////////////////////////////////////////////////

void NodeTransformController::update( float timeElapsed )
{
   // calculate the parameter change factor ( relative to the camera orientation )
   float movementSpeed = 0.1f * timeElapsed;
   D3DXVECTOR2 valChange( 0, 0 );
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
      valChange = m_uic->getMouseSpeed() * movementSpeed;
   }

   // transform the manipulated nodes
   unsigned int count = m_nodes.size();
   for( unsigned int i = 0; i < count; ++i )
   {
      transformNode( *m_nodes[i], valChange );
   }
}

///////////////////////////////////////////////////////////////////////////////

void NodeTransformController::transformNode( Node& node, const D3DXVECTOR2& valChange ) const
{
   D3DXMATRIX& localMtx = node.accessLocalMtx();
   switch( m_controlMode )
   {
   case NTM_TRANSLATE:
      {
         // calculate the camera relative translation
         D3DXVECTOR3 vecX, vecY, vecZ, camPos;
         m_camera->getGlobalVectors( vecX, vecY, vecZ, camPos );

         D3DXVec3Normalize( &vecX, &vecX );
         D3DXVec3Normalize( &vecY, &vecY );

         D3DXVECTOR3 camRelVals = vecX * valChange.x - vecY * valChange.y;

         // manipulate the node
         D3DXMATRIX changeMtx;
         D3DXMatrixTranslation( &changeMtx, camRelVals.x, camRelVals.y, camRelVals.z );
         D3DXMatrixMultiply( &localMtx, &changeMtx, &localMtx );
         break;
      }

   case NTM_ROTATE:
      {
         // manipulate the node
         D3DXMATRIX changeMtx;
         D3DXMatrixRotationAxis( &changeMtx, &m_rotationAxis, valChange.x + valChange.y );
         D3DXMatrixMultiply( &localMtx, &changeMtx, &localMtx );
         break;
      }

   default:
      {
         break;
      }
   }

   // regenerate node's transformation matrix' vectors to ensure their orthogonality
   D3DXVECTOR3 rightVec = node.getRightVec();
   D3DXVECTOR3 upVec = node.getUpVec();
   D3DXVECTOR3 lookVec = node.getLookVec();

   D3DXVec3Normalize( &lookVec, &lookVec );
   D3DXVec3Cross( &rightVec, &upVec, &lookVec );
   D3DXVec3Normalize( &rightVec, &rightVec );
   D3DXVec3Cross( &upVec, &lookVec, &rightVec );
   D3DXVec3Normalize( &upVec, &upVec );

   node.setRightVec( rightVec );
   node.setUpVec( upVec );
   node.setLookVec( lookVec );
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 NodeTransformController::selectGlobalRotationAxis() const
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

   return GLOBAL_AXES[ axisIdx ];*/

   // always rotate around OY axis
   return GLOBAL_AXES[ 1 ];
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
