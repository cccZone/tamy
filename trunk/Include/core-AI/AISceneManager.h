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
   AISceneManager();
   ~AISceneManager();

   void update(float timeElapsed);

protected:
   void add(AgentNode& agent);
   void remove(AgentNode& agent);
};

///////////////////////////////////////////////////////////////////////////////
