#include "core-Renderer\DynamicNodesStorage.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\GraphicalNodesProcessor.h"
#include "core\Frustum.h"


///////////////////////////////////////////////////////////////////////////////

DynamicNodesStorage::DynamicNodesStorage(SpatialStorage<RenderableNode>* storage)
: m_storage(storage)
, m_nodesProcessor(new GraphicalNodesProcessor())
{
}

///////////////////////////////////////////////////////////////////////////////

DynamicNodesStorage::~DynamicNodesStorage()
{
   delete m_nodesProcessor;
   m_nodesProcessor = NULL;

   delete m_storage;
   m_storage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DynamicNodesStorage::query(Camera& camera,
                                Array<Renderable*>& outRenderables)
{
   // render the scene
   m_visibleNodes.clear();

   Frustum cameraFrustum = camera.getFrustum();
   m_storage->query(cameraFrustum, m_visibleNodes);

   D3DXMATRIX cameraMtx = camera.getGlobalMtx();
   D3DXVECTOR3 cameraPos(cameraMtx._41, cameraMtx._42, cameraMtx._43);

   (*m_nodesProcessor)(m_visibleNodes, cameraFrustum, cameraPos, outRenderables);
}

///////////////////////////////////////////////////////////////////////////////
