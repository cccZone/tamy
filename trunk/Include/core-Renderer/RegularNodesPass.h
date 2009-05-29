#pragma once

#include "core-Renderer\RenderingPass.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingProcessor;

///////////////////////////////////////////////////////////////////////////////

class RegularNodesPass : public RenderingPass
{
private:
   RenderingProcessor* m_renderingProcessor;

public:
   RegularNodesPass();
   ~RegularNodesPass();

   void operator()(VisualSceneManager& sceneManager, Renderer& renderer);
};

///////////////////////////////////////////////////////////////////////////////
