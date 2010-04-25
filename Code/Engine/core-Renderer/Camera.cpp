#include "core-Renderer\Camera.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\ProjCalc3D.h"
#include "core\Frustum.h"
#include "core\Ray.h"
#include "core-Renderer\Renderer.h"


///////////////////////////////////////////////////////////////////////////////

Camera::Camera(const std::string& name, Renderer& renderer)
      : Node(name),
      m_renderer(renderer),
      m_fov(D3DXToRadian(60)),
      m_aspectRatio(1.3333f),
      m_nearZPlane(1.01f),
      m_farZPlane(5000.0f),
      m_mtxProjectionDirty(true)
{
   m_renderer.attachObserver(*this);

   D3DXMatrixIdentity(&m_mtxIdentity);
   D3DXMatrixIdentity(&m_mtxView);
   D3DXMatrixIdentity(&m_mtx3DProjection);

   m_projCalc = new ProjCalc3D();
}

///////////////////////////////////////////////////////////////////////////////

Camera::~Camera()
{
   m_renderer.detachObserver(*this);

   delete m_projCalc;
   m_projCalc = NULL;
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

void Camera::setProjectionCalculator(ProjectionCalculator* projCalc)
{
   if (projCalc == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a ProjectionCalculator instance");
   }

   delete m_projCalc;
   m_projCalc = projCalc;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& Camera::getViewMtx() 
{
   updateViewMtx();
   return m_mtxView;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& Camera::getProjectionMtx() 
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
   m_fov = D3DXToRadian(angle);
   m_mtxProjectionDirty = true;
}

///////////////////////////////////////////////////////////////////////////////

float Camera::getFOV() const 
{
   return D3DXToDegree(m_fov);
}

///////////////////////////////////////////////////////////////////////////////

void Camera::updateViewMtx()
{
   const D3DXMATRIX& globalMtx = getGlobalMtx();
   D3DXVECTOR3 rightVec(globalMtx._11, globalMtx._12, globalMtx._13);
   D3DXVECTOR3 upVec   (globalMtx._21, globalMtx._22, globalMtx._23);
   D3DXVECTOR3 lookVec (globalMtx._31, globalMtx._32, globalMtx._33);
   D3DXVECTOR3 position(globalMtx._41, globalMtx._42, globalMtx._43);

   // create a view matrix
   D3DXMatrixIdentity(&m_mtxView);
   m_mtxView._11 = rightVec.x; m_mtxView._12 = upVec.x; m_mtxView._13 = lookVec.x; 
   m_mtxView._21 = rightVec.y; m_mtxView._22 = upVec.y; m_mtxView._23 = lookVec.y;
   m_mtxView._31 = rightVec.z; m_mtxView._32 = upVec.z; m_mtxView._33 = lookVec.z;
   m_mtxView._41 = -D3DXVec3Dot(&position, &rightVec);
   m_mtxView._42 = -D3DXVec3Dot(&position, &upVec);
   m_mtxView._43 = -D3DXVec3Dot(&position, &lookVec);
}

///////////////////////////////////////////////////////////////////////////////

void Camera::updateProjectionMtx()
{  
   m_mtx3DProjection = m_projCalc->calculate(m_fov, m_aspectRatio,
                                             m_nearZPlane, m_farZPlane);
  
   m_mtxProjectionDirty = false;
}

///////////////////////////////////////////////////////////////////////////////

Frustum Camera::getFrustum()
{
   D3DXMATRIX combinedMtx = getViewMtx();
   D3DXMATRIX projMtx = getProjectionMtx();
   D3DXMatrixMultiply(&combinedMtx, &combinedMtx, &projMtx);

   Frustum frustrum;

   frustrum.planes[FP_BOTTOM].a = -(combinedMtx._14 + combinedMtx._12);
   frustrum.planes[FP_BOTTOM].b = -(combinedMtx._24 + combinedMtx._22);
   frustrum.planes[FP_BOTTOM].c = -(combinedMtx._34 + combinedMtx._32);
   frustrum.planes[FP_BOTTOM].d = -(combinedMtx._44 + combinedMtx._42);
   D3DXPlaneNormalize(&(frustrum.planes[FP_BOTTOM]), &(frustrum.planes[FP_BOTTOM]));

   frustrum.planes[FP_TOP].a = -(combinedMtx._14 - combinedMtx._12);
   frustrum.planes[FP_TOP].b = -(combinedMtx._24 - combinedMtx._22);
   frustrum.planes[FP_TOP].c = -(combinedMtx._34 - combinedMtx._32);
   frustrum.planes[FP_TOP].d = -(combinedMtx._44 - combinedMtx._42);
   D3DXPlaneNormalize(&(frustrum.planes[FP_TOP]), &(frustrum.planes[FP_TOP]));

   frustrum.planes[FP_LEFT].a  = -(combinedMtx._14 + combinedMtx._11);
   frustrum.planes[FP_LEFT].b  = -(combinedMtx._24 + combinedMtx._21);
   frustrum.planes[FP_LEFT].c  = -(combinedMtx._34 + combinedMtx._31);
   frustrum.planes[FP_LEFT].d  = -(combinedMtx._44 + combinedMtx._41);
   D3DXPlaneNormalize(&(frustrum.planes[FP_LEFT]), &(frustrum.planes[FP_LEFT]));

   frustrum.planes[FP_RIGHT].a = -(combinedMtx._14 - combinedMtx._11);
   frustrum.planes[FP_RIGHT].b = -(combinedMtx._24 - combinedMtx._21);
   frustrum.planes[FP_RIGHT].c = -(combinedMtx._34 - combinedMtx._31);
   frustrum.planes[FP_RIGHT].d = -(combinedMtx._44 - combinedMtx._41);
   D3DXPlaneNormalize(&(frustrum.planes[FP_RIGHT]), &(frustrum.planes[FP_RIGHT]));

   frustrum.planes[FP_FAR].a   = -(combinedMtx._14 - combinedMtx._13);
   frustrum.planes[FP_FAR].b   = -(combinedMtx._24 - combinedMtx._23);
   frustrum.planes[FP_FAR].c   = -(combinedMtx._34 - combinedMtx._33);
   frustrum.planes[FP_FAR].d   = -(combinedMtx._44 - combinedMtx._43);
   D3DXPlaneNormalize(&(frustrum.planes[FP_FAR]), &(frustrum.planes[FP_FAR]));

   frustrum.planes[FP_NEAR].a   = -combinedMtx._13;
   frustrum.planes[FP_NEAR].b   = -combinedMtx._23;
   frustrum.planes[FP_NEAR].c   = -combinedMtx._33;
   frustrum.planes[FP_NEAR].d   = -combinedMtx._43;
   D3DXPlaneNormalize(&(frustrum.planes[FP_NEAR]), &(frustrum.planes[FP_NEAR]));

   return frustrum;
}

///////////////////////////////////////////////////////////////////////////////

Ray Camera::createRay(float viewportX, float viewportY)
{
   D3DXMATRIX globalMtx = getGlobalMtx();

   D3DXVECTOR3 origin(globalMtx._41, globalMtx._42, globalMtx._43);

   // now I treat the mouse position as if it was located on the near clipping plane
   D3DXVECTOR3 mouseOnNearPlane(viewportX, viewportY, -m_nearZPlane);

   // these 3d coords are in the viewport space - now I need to transform them into world space
   D3DXMATRIX combinedMtx = getViewMtx();
   D3DXMATRIX projMtx = getProjectionMtx();
   D3DXMatrixMultiply(&combinedMtx, &combinedMtx, &projMtx);
   D3DXMatrixInverse(&combinedMtx, NULL, &combinedMtx);

   // once I have the backwards transformation, I use it on the 3D mouse coord
   D3DXVec3TransformCoord(&mouseOnNearPlane, &mouseOnNearPlane, &combinedMtx);

   // now I just need to find the vector between this point and the camera world space position and normalize it, and I should get my direction:
   D3DXVECTOR3 direction = mouseOnNearPlane - origin;

   return Ray(origin, direction);
}

///////////////////////////////////////////////////////////////////////////////
