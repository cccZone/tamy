#pragma once

#include "core-Renderer\RenderingPass.h"
#include "core-Renderer\RenderingCommand.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class RenderingProcessor;

///////////////////////////////////////////////////////////////////////////////

class RegularNodesPass : public RenderingPass
{
private:
   RenderingProcessor* m_renderingProcessor;

   DWORD m_commandsArraySize;
   RenderingCommand* m_renderingCommands;

public:
   RegularNodesPass();
   ~RegularNodesPass();

   void operator()(SceneManager& sceneManager, Renderer& renderer);
};

///////////////////////////////////////////////////////////////////////////////
