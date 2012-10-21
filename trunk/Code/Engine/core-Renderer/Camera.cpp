#include "core-Renderer\Camera.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\Renderer.h"
#include "core\Frustum.h"
#include "core\Ray.h"
#include "core\Assert.h"
#include "core\Matrix.h"
#include "core\MatrixUtils.h"
#include "core\MathDefs.h"
#include "core\Profiler.h"


///////////////////////////////////////////////////////////////////////////////

Camera::Camera( const std::string& name, Renderer& renderer, ProjectionType projectionType )
      : Node(name)
      , m_renderer( renderer )
      , m_projectionType( projectionType )
      , m_fov( DEG2RAD( 60.0f ) )
      , m_aspectRatio( 1.3333f )
      , m_nearPlaneWidth( 1 )
      , m_nearPlaneHeight( 1 / 1.3333f )
      , m_nearZPlane( 1.01f )
      , m_farZPlane( 1024.0f )
      , m_mtxProjectionDirty( true )
{
   m_renderer.attachObserver(*this);

   m_mtxView.setIdentity();
   m_mtx3DProjection.setIdentity();
}

///////////////////////////////////////////////////////////////////////////////

Camera::~Camera()
{
   m_renderer.detachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

void Camera::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(TNodesVisitor<Camera>);
}

///////////////////////////////////////////////////////////////////////////////

void Camera::update(Renderer& renderer)
{
   setNearPlaneDimensions((float)renderer.getViewportWidth(), (float)renderer.getViewportHeight());
}

///////////////////////////////////////////////////////////////////////////////

void Camera::update(Renderer& renderer, const RendererOps& operation)
{
   if (operation == RO_RESIZE_VIEWPORT)
   {
      setNearPlaneDimensions((float)renderer.getViewportWidth(), (float)renderer.getViewportHeight());
   }
}

///////////////////////////////////////////////////////////////////////////////

void Camera::setProjection( ProjectionType projectionType )
{
   m_projectionType = projectionType;
}

///////////////////////////////////////////////////////////////////////////////

const Matrix& Camera::getViewMtx()
{
   PROFILED();

   updateViewMtx();
   return m_mtxView;
}

///////////////////////////////////////////////////////////////////////////////

const Matrix& Camera::getProjectionMtx() 
{
   PROFILED();

   if (m_mtxProjectionDirty == true)
   {
      updateProjectionMtx();
   }

   return m_mtx3DProjection;
}

///////////////////////////////////////////////////////////////////////////////

void Camera::setNearPlaneDimensions(float width, float height)
{
   if (width < 0) width = 0;
   if (height < 1) height = 1;

   m_nearPlaneWidth = width;
   m_nearPlaneHeight = height;

   m_aspectRatio = width / height; 
   m_mtxProjectionDirty = true;
}

///////////////////////////////////////////////////////////////////////////////

void Camera::getNearPlaneDimensions( float& outWidth, float& outHeight ) const
{
   outWidth = m_nearPlaneWidth;
   outHeight = m_nearPlaneHeight;
}

///////////////////////////////////////////////////////////////////////////////

void Camera::setClippingPlanes(float nearZPlane, float farZPlane) 
{
   m_nearZPlane = nearZPlane; 
   m_farZPlane = farZPlane; 
   m_mtxProjectionDirty = true;
}

///////////////////////////////////////////////////////////////////////////////

void Camera::getClippingPlanes( float& outNearZPlane, float& outFarZPlane ) const
{
   outNearZPlane = m_nearZPlane;
   outFarZPlane = m_farZPlane;
}

///////////////////////////////////////////////////////////////////////////////

void Camera::setFOV(float angle)
{
   m_fov = DEG2RAD(angle);
   m_mtxProjectionDirty = true;
}

///////////////////////////////////////////////////////////////////////////////

float Camera::getFOV() const 
{
   return RAD2DEG(m_fov);
}

///////////////////////////////////////////////////////////////////////////////

void Camera::updateViewMtx()
{
   PROFILED();

   const Matrix& globalMtx = getGlobalMtx();
   MatrixUtils::calculateViewMtx( globalMtx, m_mtxView );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::updateProjectionMtx()
{  
   PROFILED();

   switch( m_projectionType )
   {
   case PT_PERSPECTIVE:
      {
         MatrixUtils::generatePrespectiveProjection( m_fov, m_aspectRatio, m_nearZPlane, m_farZPlane, m_mtx3DProjection );
         break;
      }

   case PT_ORTHO:
      {
         // we want the viewport to span from -1 to 1 on both axes, so we specify 2 as the size of each of the viewport dimensions
         MatrixUtils::generateOrthogonalProjection( m_nearPlaneWidth, m_nearPlaneHeight, m_nearZPlane, m_farZPlane, m_mtx3DProjection );
         break;
      }
   }

   m_mtxProjectionDirty = false;
}

///////////////////////////////////////////////////////////////////////////////

void Camera::calculateFrustum( Frustum& outFrustum )
{
   const Matrix& viewMtx = getViewMtx();
   const Matrix& projMtx = getProjectionMtx();

   Matrix combinedMtx;
   combinedMtx.setMul( viewMtx, projMtx );
   combinedMtx.transpose();
   Vector planeVec;

   planeVec.setAdd( combinedMtx.position(), combinedMtx.sideVec() );
   planeVec.store( outFrustum.planes[FP_LEFT].m_quad );
   outFrustum.planes[FP_LEFT].normalize();

   planeVec.setSub( combinedMtx.position(), combinedMtx.sideVec() );
   planeVec.store( outFrustum.planes[FP_RIGHT].m_quad );
   outFrustum.planes[FP_RIGHT].normalize();

   planeVec.setAdd( combinedMtx.position(), combinedMtx.upVec() );
   planeVec.store( outFrustum.planes[FP_BOTTOM].m_quad );
   outFrustum.planes[FP_BOTTOM].normalize();

   planeVec.setSub( combinedMtx.position(), combinedMtx.upVec() );
   planeVec.store( outFrustum.planes[FP_TOP].m_quad );
   outFrustum.planes[FP_TOP].normalize();

   planeVec.setSub( combinedMtx.position(), combinedMtx.forwardVec() );
   planeVec.store( outFrustum.planes[FP_FAR].m_quad );
   outFrustum.planes[FP_FAR].normalize();

   combinedMtx.forwardVec().store( outFrustum.planes[FP_NEAR].m_quad );
   outFrustum.planes[FP_NEAR].normalize();
}

///////////////////////////////////////////////////////////////////////////////

void Camera::createRay( float viewportX, float viewportY, Ray& outRay )
{
   // now I treat the mouse position as if it was located on the near clipping plane
   Vector mouseOnNearPlane;
   mouseOnNearPlane.set( viewportX, viewportY, -m_nearZPlane );

   // these 3d coords are in the viewport space - now I need to transform them into world space
   const Matrix& viewMtx = getViewMtx();
   const Matrix& projMtx = getProjectionMtx();

   Matrix combinedMtx;
   combinedMtx.setMul( viewMtx, projMtx );
   combinedMtx.invert();

   // once I have the backwards transformation, I use it on the 3D mouse coord
   Vector projMouseOnNearPlane;
   combinedMtx.transform( mouseOnNearPlane, projMouseOnNearPlane );

   // now I just need to find the vector between this point and the camera world space position and normalize it, and I should get my direction:
   const Matrix& globalMtx = getGlobalMtx();
   outRay.origin = globalMtx.position();
   outRay.direction.setSub( projMouseOnNearPlane, outRay.origin );
   outRay.direction.normalize();
}

///////////////////////////////////////////////////////////////////////////////

void Camera::lookAt( Node& node, const FastFloat& distance )
{
   const Matrix& targetNodeMtx = node.getGlobalMtx();
   const Vector& targetNodePos = targetNodeMtx.position();
   const Vector& targetNodeLookVec = targetNodeMtx.forwardVec();

   // I used to normalize the look vector here, but is there a need to do it really? we assume
   // it's always normalized
   ASSERT_MSG( targetNodeLookVec.isNormalized(), "Look vector not normalized" );
   FastFloat negDist;
   negDist.setNeg( distance );
   Vector newPosition;
   newPosition.setMul( targetNodeLookVec, negDist );
   newPosition.add( targetNodePos );

   Matrix lookAtMtx;
   MatrixUtils::generateLookAtLH( newPosition, targetNodePos, Quad_0100, lookAtMtx );
   setLocalMtx( lookAtMtx );
}

///////////////////////////////////////////////////////////////////////////////
