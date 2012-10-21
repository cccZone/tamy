#include "GizmoAxis.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-Renderer/LitVertex.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/VertexShader.h"
#include "core-Renderer/Defines.h"
#include "core/Math.h"

///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT(GizmoAxis)
   PARENT( Geometry )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

GizmoAxis::GizmoAxis( byte axisIdx, SpatialEntity& editedNode, Camera& activeCamera )
   : Geometry( "gizmoAxis" )
   , m_axisIdx( axisIdx )
   , m_editedNode( editedNode )
   , m_activeCamera( activeCamera )
   , m_operation( NULL )
{
   initialize();  
}

///////////////////////////////////////////////////////////////////////////////

GizmoAxis::~GizmoAxis()
{
   m_vertexShader = NULL;
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
   const Matrix& manipulationMatrix = m_editedNode.getGlobalMtx();
   const Vector& manipulationAxis = manipulationMatrix.getRow( m_axisIdx );

   FastFloat transformationValue = Float_0;
   {
      Matrix viewProjMtx;
      viewProjMtx.setMul( m_activeCamera.getViewMtx(), m_activeCamera.getProjectionMtx() );
      Vector viewportSpaceManipulationAxis;
      viewProjMtx.transformNorm( manipulationAxis, viewportSpaceManipulationAxis );
      transformationValue = viewportSpaceManipulationAxis.dot( viewportSpaceTransformation );
   }

   Matrix transformationMtx;
   m_operation->transformManipulatedNodes( Matrix::IDENTITY.getRow( m_axisIdx ), transformationValue, transformationMtx );

   // we're multiplying transformationMtx*nodeMtx, because we want to manipulate the object in it's LOCAL SPACE.
   // If you want to manipulate it in WORLD SPACE, use 'mul' instead
   Matrix& nodeMtx = m_editedNode.accessLocalMtx();
   nodeMtx.preMul( transformationMtx );

   // notify the object that it's value has externally changed ( other editors might be watching )
   m_editedNode.notifyPropertyChange( "m_localMtx" );
}

///////////////////////////////////////////////////////////////////////////////

void GizmoAxis::update( float timeElapsed )
{
   // synchronize axis position with object's position
   const Matrix& nodeMtx = m_editedNode.getGlobalMtx();
   setLocalMtx( nodeMtx );
}

///////////////////////////////////////////////////////////////////////////////

RCBindVertexShader* GizmoAxis::onPreRender( Renderer& renderer )
{
   if ( !m_vertexShader )
   {
      return NULL;
   }

   Camera& camera = renderer.getActiveCamera();
   Matrix nodeMtx = m_editedNode.getGlobalMtx();

   Vector cameraPos;
   camera.getPosition( cameraPos );
   Vector dirToCamera;
   dirToCamera.setSub( cameraPos, nodeMtx.position() );
   const FastFloat scale = dirToCamera.length();

   Matrix scaleMtx;
   scaleMtx.scaleUniform( scale );
   nodeMtx.preMul( scaleMtx );

   // setup the vertex shader
   RCBindVertexShader* comm = new ( renderer() ) RCBindVertexShader( *m_vertexShader, renderer );
   {
      Matrix worldViewMtx;
      worldViewMtx.setMul( nodeMtx, camera.getViewMtx() );

      Matrix worldViewProjMtx;
      worldViewProjMtx.setMul( worldViewMtx, camera.getProjectionMtx() );

      comm->setMtx( "g_mWorldView", worldViewMtx );
      comm->setMtx( "g_mWorldViewProj", worldViewProjMtx );

      comm->setMtx( "g_mWorldView", nodeMtx );
      comm->setMtx( "g_mWorldViewProj", worldViewProjMtx );
   }

   return comm;
}

///////////////////////////////////////////////////////////////////////////////

void GizmoAxis::onPostRender( Renderer& renderer )
{
   if ( m_vertexShader )
   {
      new ( renderer() ) RCUnbindVertexShader( *m_vertexShader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void GizmoAxis::initialize()
{
   ResourcesManager& rm = ResourcesManager::getInstance();
   static FilePath shaderName( SHADERS_DIR "VertexShaders/staticGeometry.tvsh" );

   m_vertexShader = rm.create< VertexShader >( shaderName );
   ASSERT_MSG( m_vertexShader, "Vertex shader could not be loaded" );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void TranslationGizmoOp::transformManipulatedNodes( const Vector& manipulationAxis, const FastFloat& transformationValue, Matrix& outTransformationMtx ) const
{
   Vector translation;
   translation.setMul( manipulationAxis, transformationValue );

   outTransformationMtx.setTranslation( translation );
}

///////////////////////////////////////////////////////////////////////////////

void RotationGizmoOp::transformManipulatedNodes( const Vector& manipulationAxis, const FastFloat& transformationValue, Matrix& outTransformationMtx ) const
{
   Quaternion rotQ;
   rotQ.setAxisAngle( manipulationAxis, transformationValue );

   outTransformationMtx.setRotation( rotQ );
}

///////////////////////////////////////////////////////////////////////////////

void ScalingGizmoOp::transformManipulatedNodes( const Vector& manipulationAxis, const FastFloat& transformationValue, Matrix& outTransformationMtx ) const
{
   Vector scaleVec;
   scaleVec.setMul( manipulationAxis, transformationValue );
   scaleVec.add( Vector_ONE );

   outTransformationMtx.scale( scaleVec );
}

///////////////////////////////////////////////////////////////////////////////
