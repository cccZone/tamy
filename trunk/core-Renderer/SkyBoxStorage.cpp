#include "core-Renderer\SkyBoxStorage.h"
#include "core-Renderer\SkyBoxSide.h"
#include "core-Renderer\Camera.h"


///////////////////////////////////////////////////////////////////////////////

SkyBoxStorage::SkyBoxStorage(SkyBoxSide* left, SkyBoxSide* right,
                             SkyBoxSide* top, SkyBoxSide* bottom,
                             SkyBoxSide* front, SkyBoxSide* back)
: m_sides(6)
{
   m_sides.push_back(left);
   m_sides.push_back(right);
   m_sides.push_back(top);
   m_sides.push_back(bottom);
   m_sides.push_back(front);
   m_sides.push_back(back);
}

///////////////////////////////////////////////////////////////////////////////

SkyBoxStorage::~SkyBoxStorage()
{
   for (unsigned int i = 0; i < 6; ++i)
   {
      delete m_sides[i];
   }
}

///////////////////////////////////////////////////////////////////////////////

/// CAUTION - we're changing a shared renderable use (the same storage can
/// be accessed from many places - and if each provides a different camera
/// with a different matrix, the returned renderables will have their
/// rendering matrices set based on the last camera used.
/// Should that ever be a problem - change the SkyBoxSide class to be immutable
/// and work from there
void SkyBoxStorage::query(Camera& camera, Array<Renderable*>& outRenderables)
{
   SkyBoxSide* renderable = NULL;

   D3DXMATRIX mtx;
   D3DXMatrixIdentity(&mtx);

   D3DXMATRIX cameraGlobalMtx = camera.getGlobalMtx();
   mtx._41 = cameraGlobalMtx._41;
   mtx._42 = cameraGlobalMtx._42;
   mtx._43 = cameraGlobalMtx._43;

   for (unsigned int i = 0; i < 6; ++i)
   {
      renderable = m_sides[i];

      renderable->setGlobalMtx(mtx);
      outRenderables.push_back(renderable);
   }
}

///////////////////////////////////////////////////////////////////////////////
