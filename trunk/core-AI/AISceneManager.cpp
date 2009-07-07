#include "core-AI\AISceneManager.h"
#include "core-AI\MessageDispatcher.h"


///////////////////////////////////////////////////////////////////////////////

AISceneManager::AISceneManager(unsigned int maxElemsPerSector, float worldSize)
      : SceneAspectManager<AgentNode>(maxElemsPerSector, worldSize)
{
   REGISTER_SCENE_ASPECT(AgentNode);
   m_messageDispatcher = new MessageDispatcher();
}

///////////////////////////////////////////////////////////////////////////////

AISceneManager::~AISceneManager()
{
   delete m_messageDispatcher;
   m_messageDispatcher = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void AISceneManager::onAdd(AgentNode& agent)
{
   m_agents.push_back(&agent);
   agent.setMessageDispatcher(*m_messageDispatcher);
}

///////////////////////////////////////////////////////////////////////////////

void AISceneManager::onRemove(AgentNode& agent)
{
   int idx = m_agents.find(&agent);
   if (idx != EOA)
   {
      agent.resetMessageDispatcher();
      m_agents.remove(idx);
   }
}

///////////////////////////////////////////////////////////////////////////////

void AISceneManager::update(float timeElapsed)
{
   unsigned int size = m_agents.size();
   for (unsigned int i = 0; i < size; ++i)
   {
      m_agents[i]->update();
   }

   m_messageDispatcher->update(timeElapsed);
}

///////////////////////////////////////////////////////////////////////////////