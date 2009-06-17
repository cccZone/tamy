#pragma once

#include "core-Renderer\RenderingPass.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingProcessor;
class GraphicalNodesProcessor;
class AbstractGraphicalNode;

///////////////////////////////////////////////////////////////////////////////

class DrawingPass : public RenderingPass
{
private:
   GraphicalNodesProcessor* m_nodesProcessor;
   RenderingProcessor* m_renderingProcessor;

   Array<AbstractGraphicalNode*> m_visibleNodes;
   Array<AbstractGraphicalNode*> m_nodesToProcess;

public:
   DrawingPass();
   ~DrawingPass();

   void operator()(VisualSceneManager& sceneManager, Renderer& renderer);
};

///////////////////////////////////////////////////////////////////////////////
