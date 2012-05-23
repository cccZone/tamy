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
      , m_farZPlane( 5000.0f )
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
   updateViewMtx();
   return m_mtxView;
}

///////////////////////////////////////////////////////////////////////////////

const Matrix& Camera::getProjectionMtx() 
{
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
   const Matrix& globalMtx = getGlobalMtx();
   Vector rightVec, upVec, lookVec, position;
   globalMtx.getVectors( rightVec, upVec, lookVec, position );

   // create a view matrix
   m_mtxView = Matrix::IDENTITY;
   m_mtxView.m[0][0] = rightVec.x; m_mtxView.m[0][1] = upVec.x; m_mtxView.m[0][2] = lookVec.x; 
   m_mtxView.m[1][0] = rightVec.y; m_mtxView.m[1][1] = upVec.y; m_mtxView.m[1][2] = lookVec.y;
   m_mtxView.m[2][0] = rightVec.z; m_mtxView.m[2][1] = upVec.z; m_mtxView.m[2][2] = lookVec.z;
   m_mtxView.m[3][0] = -position.dot( rightVec );
   m_mtxView.m[3][1] = -position.dot( upVec );
   m_mtxView.m[3][2] = -position.dot( lookVec );
}

///////////////////////////////////////////////////////////////////////////////

void Camera::updateProjectionMtx()
{  
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
         MatrixUtils::generateOrthogonalProjection( 2.0f, 2.0f, m_nearZPlane, m_farZPlane, m_mtx3DProjection );
         break;
      }
   }

   m_mtxProjectionDirty = false;
}

///////////////////////////////////////////////////////////////////////////////

Frustum Camera::getFrustum()
{
   const Matrix& viewMtx = getViewMtx();
   const Matrix& projMtx = getProjectionMtx();

   Matrix combinedMtx;
   combinedMtx.setMul( viewMtx, projMtx );

   Frustum frustrum;

   frustrum.planes[FP_BOTTOM].a = -(combinedMtx.m14 + combinedMtx.m12);
   frustrum.planes[FP_BOTTOM].b = -(combinedMtx.m24 + combinedMtx.m22);
   frustrum.planes[FP_BOTTOM].c = -(combinedMtx.m34 + combinedMtx.m32);
   frustrum.planes[FP_BOTTOM].d = -(combinedMtx.m44 + combinedMtx.m42);
   frustrum.planes[FP_BOTTOM].normalize();

   frustrum.planes[FP_TOP].a = -(combinedMtx.m14 - combinedMtx.m12);
   frustrum.planes[FP_TOP].b = -(combinedMtx.m24 - combinedMtx.m22);
   frustrum.planes[FP_TOP].c = -(combinedMtx.m34 - combinedMtx.m32);
   frustrum.planes[FP_TOP].d = -(combinedMtx.m44 - combinedMtx.m42);
   frustrum.planes[FP_TOP].normalize();

   frustrum.planes[FP_LEFT].a  = -(combinedMtx.m14 + combinedMtx.m11);
   frustrum.planes[FP_LEFT].b  = -(combinedMtx.m24 + combinedMtx.m21);
   frustrum.planes[FP_LEFT].c  = -(combinedMtx.m34 + combinedMtx.m31);
   frustrum.planes[FP_LEFT].d  = -(combinedMtx.m44 + combinedMtx.m41);
   frustrum.planes[FP_LEFT].normalize();

   frustrum.planes[FP_RIGHT].a = -(combinedMtx.m14 - combinedMtx.m11);
   frustrum.planes[FP_RIGHT].b = -(combinedMtx.m24 - combinedMtx.m21);
   frustrum.planes[FP_RIGHT].c = -(combinedMtx.m34 - combinedMtx.m31);
   frustrum.planes[FP_RIGHT].d = -(combinedMtx.m44 - combinedMtx.m41);
   frustrum.planes[FP_RIGHT].normalize();

   frustrum.planes[FP_FAR].a   = -(combinedMtx.m14 - combinedMtx.m13);
   frustrum.planes[FP_FAR].b   = -(combinedMtx.m24 - combinedMtx.m23);
   frustrum.planes[FP_FAR].c   = -(combinedMtx.m34 - combinedMtx.m33);
   frustrum.planes[FP_FAR].d   = -(combinedMtx.m44 - combinedMtx.m43);
   frustrum.planes[FP_FAR].normalize();

   frustrum.planes[FP_NEAR].a   = -combinedMtx.m13;
   frustrum.planes[FP_NEAR].b   = -combinedMtx.m23;
   frustrum.planes[FP_NEAR].c   = -combinedMtx.m33;
   frustrum.planes[FP_NEAR].d   = -combinedMtx.m43;
   frustrum.planes[FP_NEAR].normalize();


   return frustrum;
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
