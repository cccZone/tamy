#pragma once

#include "core\AbstractSceneManager.h"
#include "core\TNodesVisitor.h"
#include "core\Array.h"
#include "core-AI\AgentNode.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class MessageDispatcher;

///////////////////////////////////////////////////////////////////////////////

/**
 * This scene manager manages the AI agents
 */
class AISceneManager : public AbstractSceneManager,
                       public SceneAspectManager<AgentNode>
{
private:
   Array<AgentNode*> m_agents;
   MessageDispatcher* m_messageDispatcher;

public:
   AISceneManager(unsigned int maxElemsPerSector = 64, float worldSize = 1000);
   ~AISceneManager();

   void update(float timeElapsed);

protected:
   void onAdd(AgentNode& agent);
   void onRemove(AgentNode& agent);
};

///////////////////////////////////////////////////////////////////////////////
