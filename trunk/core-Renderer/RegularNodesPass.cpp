#include "core-Renderer\RegularNodesPass.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\Camera.h"


///////////////////////////////////////////////////////////////////////////////

RegularNodesPass::RegularNodesPass()
      : m_renderingProcessor(new RenderingProcessor())
{
}

///////////////////////////////////////////////////////////////////////////////

RegularNodesPass::~RegularNodesPass()
{
   delete m_renderingProcessor;
   m_renderingProcessor = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RegularNodesPass::operator()(VisualSceneManager& sceneManager, Renderer& renderer)
{
   DWORD nodesArraySize = 0;
   AbstractGraphicalNodeP* nodes = sceneManager.getRegularGraphicalNodes(nodesArraySize);
   m_renderingProcessor->translate(nodes, nodesArraySize);
}

///////////////////////////////////////////////////////////////////////////////
