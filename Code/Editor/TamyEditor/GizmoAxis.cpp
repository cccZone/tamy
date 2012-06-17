#include "GizmoAxis.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-Renderer/LitVertex.h"
#include "core-Renderer/Camera.h"
#include "core/Math.h"


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

void GizmoAxis::transformManipulatedNodes( const Vector& viewportSpaceTransformation )
{
   if ( m_operation == NULL )
   {
      return;
   }

   // Calculate the transformation value - this value represents how far do we want
   // to transform something along this manipulation axis.
   // We want the value to be maximal when the user is sliding the mouse along the axis, and
   // become smaller until reaching zero if he moves the mouse perpendicularly to he axis.
   // Since the input 'viewportSpaceTransformation' contains the transformation values in the viewport space,
   // all we need to do is take our manipulation axis, transform it to the viewport space ( with respect
   // to the active camera ) and then calculate a dot product between our 2d manipulation axis and 
   // the viewport space transformation vector - thus calculating our transformation value.
   const Matrix& manipulationMatrix = Matrix::IDENTITY;
   const Vector& manipulationAxis = manipulationMatrix.getRow( m_axisIdx );

   float transformationValue = 0.0f;
   {
      Matrix viewProjMtx;
      viewProjMtx.setMul( m_activeCamera.getViewMtx(), m_activeCamera.getProjectionMtx() );
      Vector viewportSpaceManipulationAxis;
      viewProjMtx.transformNorm( manipulationAxis, viewportSpaceManipulationAxis );

      transformationValue = viewportSpaceManipulationAxis.dot( viewportSpaceTransformation );
   }

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
   Quaternion rotQ;
   rotQ.setAxisAngle( manipulationAxis, transformationValue );

   outTransformationMtx.setRotation( rotQ );
}

///////////////////////////////////////////////////////////////////////////////

void ScalingGizmoOp::transformManipulatedNodes( const Vector& manipulationAxis, float transformationValue, Matrix& outTransformationMtx ) const
{
   Vector scaleVec;
   scaleVec.setMul( manipulationAxis, transformationValue ).add( Vector::ONE );

   outTransformationMtx.scale( scaleVec );
}

///////////////////////////////////////////////////////////////////////////////
