#ifndef _SORTING_RENDERABLES_STORAGE_H
#error "This file can only be included from SortingRenderablesStorage.h"
#else

#include "core-Renderer\Camera.h"
#include "core-Renderer\RenderableNode.h"
#include "core\Frustum.h"


///////////////////////////////////////////////////////////////////////////////

template<typename InRenderable>
SortingRenderablesStorage<InRenderable>::SortingRenderablesStorage(
                                       SpatialStorage<InRenderable>& storage)
: m_storage(storage)
, m_nodesProcessor(new RenderablesSorter<InRenderable>())
{
}

///////////////////////////////////////////////////////////////////////////////

template<typename InRenderable>
SortingRenderablesStorage<InRenderable>::~SortingRenderablesStorage()
{
   delete m_nodesProcessor;
   m_nodesProcessor = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template<typename InRenderable>
void SortingRenderablesStorage<InRenderable>::query(Camera& camera,
                                                    Array<Renderable*>& outRenderables)
{
   // render the scene
   m_visibleNodes.clear();

   Frustum cameraFrustum = camera.getFrustum();
   m_storage.query(cameraFrustum, m_visibleNodes);

   D3DXMATRIX cameraMtx = camera.getGlobalMtx();
   D3DXVECTOR3 cameraPos(cameraMtx._41, cameraMtx._42, cameraMtx._43);

   (*m_nodesProcessor)(m_visibleNodes, cameraFrustum, cameraPos, outRenderables);
}

///////////////////////////////////////////////////////////////////////////////

#endif // _SORTING_RENDERABLES_STORAGE_H
