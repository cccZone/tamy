#pragma once

#include "core-Renderer\RenderingPass.h"
#include "core-Renderer\RenderingCommand.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class RenderingProcessor;

///////////////////////////////////////////////////////////////////////////////

class TransparentNodesPass : public RenderingPass
{
private:
   RenderingProcessor* m_renderingProcessor;

   DWORD m_commandsArraySize;
   RenderingCommand* m_renderingCommands;

public:
   TransparentNodesPass();
   ~TransparentNodesPass();

   void operator()(VisualSceneManager& sceneManager, Renderer& renderer);
};

///////////////////////////////////////////////////////////////////////////////
