#include "core-Renderer\Camera.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\ProjCalc3D.h"


///////////////////////////////////////////////////////////////////////////////

Camera::Camera(const std::string& name)
      : Node(name),
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

const D3DMATRIX& Camera::getProjectionMtx3D() 
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
