#include "core-Renderer\DrawingPass.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\Camera.h"


///////////////////////////////////////////////////////////////////////////////

DrawingPass::DrawingPass()
      : m_renderingProcessor(new RenderingProcessor())
{
}

///////////////////////////////////////////////////////////////////////////////

DrawingPass::~DrawingPass()
{
   delete m_renderingProcessor;
   m_renderingProcessor = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DrawingPass::operator()(VisualSceneManager& sceneManager, Renderer& renderer)
{
   DWORD nodesArraySize = 0;
   AbstractGraphicalNodeP* nodes = sceneManager.getNodes(nodesArraySize);
   m_renderingProcessor->translate(nodes, nodesArraySize);
}

///////////////////////////////////////////////////////////////////////////////
