#include "core-Renderer\Camera.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\ProjCalc3D.h"
#include "core\Frustum.h"


///////////////////////////////////////////////////////////////////////////////

Camera::Camera(const std::string& name)
      : Node(name, true),
      m_fov(D3DXToRadian(60)),
      m_aspectRatio(1.3333f),
      m_nearZPlane(1.01f),
      m_farZPlane(5000.0f),
      m_mtxProjectionDirty(true)
{
   D3DXMatrixIdentity(&m_mtxIdentity);
   D3DXMatrixIdentity(&m_mtxView);
   D3DXMatrixIdentity(&m_mtx3DProjection);

   m_projCalc = new ProjCalc3D();
}

///////////////////////////////////////////////////////////////////////////////

Camera::~Camera()
{
   delete m_projCalc;
   m_projCalc = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Camera::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(TNodesVisitor<Camera>);
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

const D3DMATRIX& Camera::getViewMtx() 
{
   updateViewMtx();
   return m_mtxView;
}

///////////////////////////////////////////////////////////////////////////////

const D3DMATRIX& Camera::getProjectionMtx() 
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

Frustum Camera::getFrustrum()
{
   D3DXMATRIX combinedMtx = getViewMtx();
   D3DXMATRIX projMtx = getProjectionMtx();
   D3DXMatrixMultiply(&combinedMtx, &combinedMtx, &projMtx);

   Frustum frustrum;

   frustrum.planes[0].a = -(combinedMtx._14 + combinedMtx._12);
   frustrum.planes[0].b = -(combinedMtx._24 + combinedMtx._22);
   frustrum.planes[0].c = -(combinedMtx._34 + combinedMtx._32);
   frustrum.planes[0].d = -(combinedMtx._44 + combinedMtx._42);
   D3DXPlaneNormalize(&(frustrum.planes[0]), &(frustrum.planes[0]));

   frustrum.planes[1].a = -(combinedMtx._14 - combinedMtx._12);
   frustrum.planes[1].b = -(combinedMtx._24 - combinedMtx._22);
   frustrum.planes[1].c = -(combinedMtx._34 - combinedMtx._32);
   frustrum.planes[1].d = -(combinedMtx._44 - combinedMtx._42);
   D3DXPlaneNormalize(&(frustrum.planes[1]), &(frustrum.planes[1]));

   frustrum.planes[2].a  = -(combinedMtx._14 + combinedMtx._11);
   frustrum.planes[2].b  = -(combinedMtx._24 + combinedMtx._21);
   frustrum.planes[2].c  = -(combinedMtx._34 + combinedMtx._31);
   frustrum.planes[2].d  = -(combinedMtx._44 + combinedMtx._41);
   D3DXPlaneNormalize(&(frustrum.planes[2]), &(frustrum.planes[2]));

   frustrum.planes[3].a = -(combinedMtx._14 - combinedMtx._11);
   frustrum.planes[3].b = -(combinedMtx._24 - combinedMtx._21);
   frustrum.planes[3].c = -(combinedMtx._34 - combinedMtx._31);
   frustrum.planes[3].d = -(combinedMtx._44 - combinedMtx._41);
   D3DXPlaneNormalize(&(frustrum.planes[3]), &(frustrum.planes[3]));

   frustrum.planes[4].a   = -(combinedMtx._14 - combinedMtx._13);
   frustrum.planes[4].b   = -(combinedMtx._24 - combinedMtx._23);
   frustrum.planes[4].c   = -(combinedMtx._34 - combinedMtx._33);
   frustrum.planes[4].d   = -(combinedMtx._44 - combinedMtx._43);
   D3DXPlaneNormalize(&(frustrum.planes[4]), &(frustrum.planes[4]));

   frustrum.planes[5].a   = -combinedMtx._13;
   frustrum.planes[5].b   = -combinedMtx._23;
   frustrum.planes[5].c   = -combinedMtx._33;
   frustrum.planes[5].d   = -combinedMtx._43;
   D3DXPlaneNormalize(&(frustrum.planes[5]), &(frustrum.planes[5]));

   return frustrum;
}

///////////////////////////////////////////////////////////////////////////////
