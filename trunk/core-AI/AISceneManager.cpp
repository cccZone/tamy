#include "core-AI\AISceneManager.h"
#include "core-AI\MessageDispatcher.h"


///////////////////////////////////////////////////////////////////////////////

AISceneManager::AISceneManager()
{
   m_addOperation = new AddOperation(*this);
   m_removeOperation = new RemoveOperation(*this);
   m_noOperation = new NoOperation();
   m_currentOperation = m_noOperation;

   m_messageDispatcher = new MessageDispatcher();
}

///////////////////////////////////////////////////////////////////////////////

AISceneManager::~AISceneManager()
{
   delete m_messageDispatcher;
   m_messageDispatcher = NULL;

   m_currentOperation = NULL;

   delete m_noOperation;
   m_noOperation = NULL;

   delete m_removeOperation;
   m_removeOperation = NULL;

   delete m_addOperation;
   m_addOperation = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void AISceneManager::addNode(Node* node)
{
   m_currentOperation = m_addOperation;
   node->accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

void AISceneManager::removeNode(Node& node)
{
   m_currentOperation = m_removeOperation;
   node.accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

void AISceneManager::visit(AgentNode& agent)
{
   m_currentOperation->perform(agent);
}

///////////////////////////////////////////////////////////////////////////////

void AISceneManager::add(AgentNode& agent)
{
   m_agents.push_back(&agent);
   agent.setMessageDispatcher(*m_messageDispatcher);
}

///////////////////////////////////////////////////////////////////////////////

void AISceneManager::remove(AgentNode& agent)
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
