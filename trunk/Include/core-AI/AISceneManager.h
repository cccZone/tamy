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
                       public TNodesVisitor<AgentNode>
{
private:
   class Operation
   {
   public:
      virtual ~Operation() {}
      
      virtual void perform(AgentNode& agent) = 0;
   };

private:
   Operation* m_addOperation;
   Operation* m_removeOperation;
   Operation* m_noOperation;
   Operation* m_currentOperation;

   Array<AgentNode*> m_agents;
   MessageDispatcher* m_messageDispatcher;

public:
   AISceneManager();
   ~AISceneManager();

   void addNode(Node* node);
   void removeNode(Node& node);

   void visit(AgentNode& agent);

   void add(AgentNode& agent);
   void remove(AgentNode& agent);

   void update(float timeElapsed);

private:
   // ---------------------------

   class AddOperation : public Operation
   {
   private:
      AISceneManager& m_controller;

   public:
      AddOperation(AISceneManager& controller) : m_controller(controller) {}

      void perform(AgentNode& agent) {m_controller.add(agent);}
   };
   friend class AddOperation;

   // ---------------------------

   class RemoveOperation : public Operation
   {
   private:
      AISceneManager& m_controller;

   public:
      RemoveOperation(AISceneManager& controller) : m_controller(controller) {}

      void perform(AgentNode& agent) {m_controller.remove(agent);}
   };
   friend class RemoveOperation;

   // ---------------------------

   class NoOperation : public Operation
   {
   public:
      NoOperation() {}
      
      void perform(AgentNode& agent) {}
   };
   friend class NoOperation;

   // ---------------------------
};

///////////////////////////////////////////////////////////////////////////////
