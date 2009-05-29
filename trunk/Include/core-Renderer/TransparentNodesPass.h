#pragma once

#include "core-Renderer\RenderingPass.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingProcessor;

///////////////////////////////////////////////////////////////////////////////

class TransparentNodesPass : public RenderingPass
{
private:
   RenderingProcessor* m_renderingProcessor;

public:
   TransparentNodesPass();
   ~TransparentNodesPass();

   void operator()(VisualSceneManager& sceneManager, Renderer& renderer);
};

///////////////////////////////////////////////////////////////////////////////
