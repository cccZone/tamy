#include "core-Renderer\DrawingPass.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\GraphicalNodesProcessor.h"
#include "core\Frustum.h"


///////////////////////////////////////////////////////////////////////////////

DrawingPass::DrawingPass()
      : m_nodesProcessor(new GraphicalNodesProcessor()),
      m_renderingProcessor(new RenderingProcessor())
{
}

///////////////////////////////////////////////////////////////////////////////

DrawingPass::~DrawingPass()
{
   delete m_nodesProcessor;
   m_nodesProcessor = NULL;

   delete m_renderingProcessor;
   m_renderingProcessor = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DrawingPass::operator()(VisualSceneManager& sceneManager, Renderer& renderer)
{
   m_visibleNodes.clear();
   m_nodesToProcess.clear();
   
   Camera&  activeCamera = sceneManager.getActiveCamera();

   Frustum cameraFrustum = activeCamera.getFrustum();
   sceneManager.query<AbstractGraphicalNode>(cameraFrustum, m_visibleNodes);
   
   D3DXMATRIX cameraMtx = activeCamera.getGlobalMtx();
   D3DXVECTOR3 cameraPos(cameraMtx._41, cameraMtx._42, cameraMtx._43);

   (*m_nodesProcessor)(m_visibleNodes, cameraFrustum, cameraPos, m_nodesToProcess);

   m_renderingProcessor->translate(m_nodesToProcess, m_nodesToProcess.size());
}

///////////////////////////////////////////////////////////////////////////////
