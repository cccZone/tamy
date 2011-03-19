#include "NodeManualEditor.h"
#include "EditEntityCommand.h"
#include "ManualEditionModes.h"
#include "tamyeditor.h"
#include "core-Renderer.h"
#include "Gizmo.h"
#include <d3dx9.h>
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   D3DXVECTOR3 globalAxes[] = {
      D3DXVECTOR3( 1, 0, 0 ),
      D3DXVECTOR3( 0, 1, 0 ),
      D3DXVECTOR3( 0, 0, 1 )
   };
}

///////////////////////////////////////////////////////////////////////////////

NodeManualEditor::NodeManualEditor( SpatialEntity& node )
: m_node( node )
, m_host( NULL )
, m_debugScene( NULL )
, m_gizmo( NULL )
, m_rotationAxis( globalAxes[0] )
{
}

///////////////////////////////////////////////////////////////////////////////

NodeManualEditor::~NodeManualEditor()
{
   if ( m_debugScene )
   {
      m_debugScene->remove( *m_gizmo );
      
      delete m_gizmo;
      m_gizmo = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void NodeManualEditor::initialize( EditEntityCommand& host )
{
   m_host = &host;

   TamyEditor& servicesMgr = host.getServicesMgr();

   // acquire an instance of the resources manager and a camera
   ResourcesManager& resMgr = servicesMgr.requestService< ResourcesManager >();
   m_camera = &servicesMgr.requestService< Camera >();
   m_debugScene = &servicesMgr.requestService< DebugScene >();

   // create the gizmo
   m_gizmo = new Gizmo( m_node );
   m_debugScene->add( *m_gizmo );
}

///////////////////////////////////////////////////////////////////////////////

void NodeManualEditor::notifyEditModeChange()
{
   if ( m_gizmo )
   {
      switch( m_host->getNodeEditionMode() )
      {
      case NEM_TRANSLATE:  m_gizmo->setMode( Gizmo::GM_TRANSLATION ); break;
      case NEM_ROTATE:     m_gizmo->setMode( Gizmo::GM_ROTATION ); break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void NodeManualEditor::startEdition( const D3DXVECTOR2& initClickPos )
{
   m_rotationAxis = calculateRotationAxis();
}

///////////////////////////////////////////////////////////////////////////////

void NodeManualEditor::edit( const D3DXVECTOR2& valChange )
{
   D3DXMATRIX& localMtx = m_node.accessLocalMtx();
   switch( m_host->getNodeEditionMode() )
   {
   case NEM_TRANSLATE:
      {
         // calculate camera relative values
         D3DXVECTOR3 vecX, vecY, vecZ, camPos;
         m_camera->getGlobalVectors( vecX, vecY, vecZ, camPos );

         D3DXVec3Normalize( &vecX, &vecX );
         D3DXVec3Normalize( &vecY, &vecY );

         D3DXVECTOR3 camRelVals = vecX * valChange.x - vecY * valChange.y;

         D3DXMATRIX changeMtx;
         D3DXMatrixTranslation( &changeMtx, camRelVals.x, camRelVals.y, camRelVals.z );
         D3DXMatrixMultiply( &localMtx, &changeMtx, &localMtx );
         break;
      }

   case NEM_ROTATE:
      {
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

   regenerateVectors();
}

///////////////////////////////////////////////////////////////////////////////

void NodeManualEditor::regenerateVectors()
{
   D3DXVECTOR3 rightVec = m_node.getRightVec();
   D3DXVECTOR3 upVec = m_node.getUpVec();
   D3DXVECTOR3 lookVec = m_node.getLookVec();

   D3DXVec3Normalize( &lookVec, &lookVec );
   D3DXVec3Cross( &rightVec, &upVec, &lookVec );
   D3DXVec3Normalize( &rightVec, &rightVec );
   D3DXVec3Cross( &upVec, &lookVec, &rightVec );
   D3DXVec3Normalize( &upVec, &upVec );

   m_node.setRightVec( rightVec );
   m_node.setUpVec( upVec );
   m_node.setLookVec( lookVec );
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 NodeManualEditor::calculateRotationAxis() const
{

   // get the direction vector from the camera to the object
   D3DXVECTOR3 objPos, camPos;
   D3DXVECTOR3 objAxes[3];
   D3DXVECTOR3 camAxes[3];
   m_camera->getGlobalVectors( camAxes[0], camAxes[1], camAxes[2], camPos );
   m_node.getGlobalVectors( objAxes[0], objAxes[1], objAxes[2], objPos );
   D3DXVECTOR3 dirToObj = objPos - camPos;
   D3DXVec3Normalize( &dirToObj, &dirToObj );

   // find the axis we're closest to and select it for rotation
   float smallestAngle = 181.f;
   int axisIdx = -1;
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

   return globalAxes[ axisIdx ];
}

///////////////////////////////////////////////////////////////////////////////
