#include "core-Renderer\TransparentNodesPass.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\Camera.h"


///////////////////////////////////////////////////////////////////////////////

TransparentNodesPass::TransparentNodesPass()
      : m_renderingProcessor(new RenderingProcessor())
{
}

///////////////////////////////////////////////////////////////////////////////

TransparentNodesPass::~TransparentNodesPass()
{
   delete m_renderingProcessor;
   m_renderingProcessor = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TransparentNodesPass::operator()(VisualSceneManager& sceneManager, Renderer& renderer)
{
   DWORD nodesArraySize = 0;
   AbstractGraphicalNodeP* nodes = sceneManager.getTransparentGraphicalNodes(nodesArraySize);
   m_renderingProcessor->translate(nodes, nodesArraySize);
}

///////////////////////////////////////////////////////////////////////////////
