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

   m_mtxIdentity = Matrix::IDENTITY;
   m_mtxView = Matrix::IDENTITY;
   m_mtx3DProjection = Matrix::IDENTITY;
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

   outFrustum.planes[FP_BOTTOM].a = -(combinedMtx.m14 + combinedMtx.m12);
   outFrustum.planes[FP_BOTTOM].b = -(combinedMtx.m24 + combinedMtx.m22);
   outFrustum.planes[FP_BOTTOM].c = -(combinedMtx.m34 + combinedMtx.m32);
   outFrustum.planes[FP_BOTTOM].d = -(combinedMtx.m44 + combinedMtx.m42);
   outFrustum.planes[FP_BOTTOM].normalize();

   outFrustum.planes[FP_TOP].a = -(combinedMtx.m14 - combinedMtx.m12);
   outFrustum.planes[FP_TOP].b = -(combinedMtx.m24 - combinedMtx.m22);
   outFrustum.planes[FP_TOP].c = -(combinedMtx.m34 - combinedMtx.m32);
   outFrustum.planes[FP_TOP].d = -(combinedMtx.m44 - combinedMtx.m42);
   outFrustum.planes[FP_TOP].normalize();

   outFrustum.planes[FP_LEFT].a  = -(combinedMtx.m14 + combinedMtx.m11);
   outFrustum.planes[FP_LEFT].b  = -(combinedMtx.m24 + combinedMtx.m21);
   outFrustum.planes[FP_LEFT].c  = -(combinedMtx.m34 + combinedMtx.m31);
   outFrustum.planes[FP_LEFT].d  = -(combinedMtx.m44 + combinedMtx.m41);
   outFrustum.planes[FP_LEFT].normalize();

   outFrustum.planes[FP_RIGHT].a = -(combinedMtx.m14 - combinedMtx.m11);
   outFrustum.planes[FP_RIGHT].b = -(combinedMtx.m24 - combinedMtx.m21);
   outFrustum.planes[FP_RIGHT].c = -(combinedMtx.m34 - combinedMtx.m31);
   outFrustum.planes[FP_RIGHT].d = -(combinedMtx.m44 - combinedMtx.m41);
   outFrustum.planes[FP_RIGHT].normalize();

   outFrustum.planes[FP_FAR].a   = -(combinedMtx.m14 - combinedMtx.m13);
   outFrustum.planes[FP_FAR].b   = -(combinedMtx.m24 - combinedMtx.m23);
   outFrustum.planes[FP_FAR].c   = -(combinedMtx.m34 - combinedMtx.m33);
   outFrustum.planes[FP_FAR].d   = -(combinedMtx.m44 - combinedMtx.m43);
   outFrustum.planes[FP_FAR].normalize();

   outFrustum.planes[FP_NEAR].a   = -combinedMtx.m13;
   outFrustum.planes[FP_NEAR].b   = -combinedMtx.m23;
   outFrustum.planes[FP_NEAR].c   = -combinedMtx.m33;
   outFrustum.planes[FP_NEAR].d   = -combinedMtx.m43;
   outFrustum.planes[FP_NEAR].normalize();
}

///////////////////////////////////////////////////////////////////////////////

void Camera::createRay( float viewportX, float viewportY, Ray& outRay )
{
   // now I treat the mouse position as if it was located on the near clipping plane
   Vector mouseOnNearPlane( viewportX, viewportY, -m_nearZPlane );

   // these 3d coords are in the viewport space - now I need to transform them into world space
   const Matrix& viewMtx = getViewMtx();
   const Matrix& projMtx = getProjectionMtx();

   Matrix combinedMtx;
   combinedMtx.setMul( viewMtx, projMtx ).invert();

   // once I have the backwards transformation, I use it on the 3D mouse coord
   Vector projMouseOnNearPlane;
   combinedMtx.transform( mouseOnNearPlane, projMouseOnNearPlane );

   // now I just need to find the vector between this point and the camera world space position and normalize it, and I should get my direction:
   const Matrix& globalMtx = getGlobalMtx();
   outRay.origin = globalMtx.position();
   outRay.direction.setSub( projMouseOnNearPlane, outRay.origin ).normalize();
}

///////////////////////////////////////////////////////////////////////////////

void Camera::lookAt( Node& node, float distance )
{
   const Matrix& targetNodeMtx = node.getGlobalMtx();
   const Vector& targetNodePos = targetNodeMtx.position();
   const Vector& targetNodeLookVec = targetNodeMtx.forwardVec();

   // I used to normalize the look vector here, but is there a need to do it really? we assume
   // it's always normalized
   ASSERT_MSG( targetNodeLookVec.isNormalized(), "Look vector not normalized" );
   Vector newPosition;
   newPosition.setMul( targetNodeLookVec, -distance ).add( targetNodePos );

   Matrix lookAtMtx;
   MatrixUtils::generateLookAtLH( newPosition, targetNodePos, Vector::OY, lookAtMtx );
   setLocalMtx( lookAtMtx );
}

///////////////////////////////////////////////////////////////////////////////
