#include "GizmoAxis.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-Renderer/LitVertex.h"
#include "core-Renderer/Camera.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT(GizmoAxis)
   PARENT( Geometry )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

GizmoAxis::GizmoAxis( byte axisIdx, SpatialEntity& editedNode, Camera& activeCamera )
   : m_axisIdx( axisIdx )
   , m_editedNode( editedNode )
   , m_activeCamera( activeCamera )
   , m_operation( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

GizmoAxis::~GizmoAxis()
{
   m_operation = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GizmoAxis::transformManipulatedNodes( const Vector& screenSpaceTransformation )
{
   if ( m_operation == NULL )
   {
      return;
   }

   // Calculate the transformation value - this value represents how far do we want
   // to transform something along this manipulation axis.
   // We want the value to be maximal when the user is sliding the mouse along the axis, and
   // become smaller until reaching zero if he moves the mouse perpendicularly to he axis.
   // Since the input 'screenSpaceTransformation' contains the transformation values in the screen space,
   // all we need to do is take our manipulation axis, transform it to the screen space ( with respect
   // to the active camera ) and then calculate a dot product between our 2d manipulation axis and 
   // the screen space transformation vector - thus calculating our transformation value.
   const Matrix& manipulationMatrix = Matrix::IDENTITY;
   const Vector& manipulationAxis = manipulationMatrix.getRow( m_axisIdx );

   float transformationValue = 0.0f;
   {
      Matrix viewProjMtx;
      viewProjMtx.setMul( m_activeCamera.getViewMtx(), m_activeCamera.getProjectionMtx() );
      Vector screenSpaceManipulationAxis;
      viewProjMtx.transformNorm( manipulationAxis, screenSpaceManipulationAxis );

      transformationValue = screenSpaceManipulationAxis.dot( screenSpaceTransformation );
   }

   // <gizmo.todo> !!!!! fix the erroneous transformationValue calculations ( sometimes instead of moving along the desired axis, the object is moving in the opposite direction )

   Matrix transformationMtx;
   m_operation->transformManipulatedNodes( manipulationAxis, transformationValue, transformationMtx );

   Matrix& nodeMtx = m_editedNode.accessLocalMtx();

   // we're multiplying transformationMtx*nodeMtx, because we want to manipulate the object in it's LOCAL SPACE.
   // If you want to manipulate it in WORLD SPACE, use 'mul' instead
   nodeMtx.preMul( transformationMtx );
}

///////////////////////////////////////////////////////////////////////////////

void GizmoAxis::onUpdate( float timeElapsed )
{
   // synchronize axis position with object's position
   const Matrix& nodeMtx = m_editedNode.getGlobalMtx();
   setLocalMtx( nodeMtx );
}

///////////////////////////////////////////////////////////////////////////////

bool GizmoAxis::onPreRender( Renderer& renderer )
{
   Camera& camera = renderer.getActiveCamera();
   Matrix nodeMtx = m_editedNode.getGlobalMtx();

   Vector cameraPos;
   camera.getPosition( cameraPos );
   Vector dirToCamera;
   dirToCamera.setSub( cameraPos, nodeMtx.position() );
   float scale = dirToCamera.length();

   nodeMtx.scaleUniform( scale );

   new ( renderer() ) RCSetVertexDeclaration( LitVertex::FVF );
   new ( renderer() ) RCSetWorldMatrix( nodeMtx );

   return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void TranslationGizmoOp::transformManipulatedNodes( const Vector& manipulationAxis, float transformationValue, Matrix& outTransformationMtx ) const
{
   Vector translation;
   translation.setMul( manipulationAxis, transformationValue );

   outTransformationMtx.setTranslation( translation );
}

///////////////////////////////////////////////////////////////////////////////

void RotationGizmoOp::transformManipulatedNodes( const Vector& manipulationAxis, float transformationValue, Matrix& outTransformationMtx ) const
{
   // <gizmo.todo> implement me
}

///////////////////////////////////////////////////////////////////////////////

void ScalingGizmoOp::transformManipulatedNodes( const Vector& manipulationAxis, float transformationValue, Matrix& outTransformationMtx ) const
{
   // <gizmo.todo> implement me
}

///////////////////////////////////////////////////////////////////////////////

/*
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
*/