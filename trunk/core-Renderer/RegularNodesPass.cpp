#include "core-Renderer\RegularNodesPass.h"
#include "core-Renderer\SceneManager.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\Camera.h"


///////////////////////////////////////////////////////////////////////////////

RegularNodesPass::RegularNodesPass()
      : m_renderingProcessor(new RenderingProcessor())
{
   m_commandsArraySize = 100000; // TODO: rozmiar tego
   m_renderingCommands = new RenderingCommand[m_commandsArraySize];
}

///////////////////////////////////////////////////////////////////////////////

RegularNodesPass::~RegularNodesPass()
{
   delete [] m_renderingCommands; 
   m_renderingCommands = NULL;
   m_commandsArraySize = 0;

   delete m_renderingProcessor;
   m_renderingProcessor = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RegularNodesPass::operator()(SceneManager& sceneManager, Renderer& renderer)
{
   DWORD nodesArraySize = 0;
   AbstractGraphicalNodeP* nodes = sceneManager.getRegularGraphicalNodes(nodesArraySize);

   DWORD commandsCount = m_renderingProcessor->translate(nodes, nodesArraySize, 
                                                         m_renderingCommands, 
                                                         m_commandsArraySize);

   for (DWORD i = 0; i < commandsCount; ++i)
   {
      (m_renderingCommands[i])();
   }
}

///////////////////////////////////////////////////////////////////////////////
