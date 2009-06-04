#pragma once

#include "core\Node.h"
#include "core-AI\MessageDispatcher.h"
#include "core-AI\Messagable.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a node with an AI logic
 */
class AgentNode : public Node, public Messagable
{
private:
   MessageDispatcher* m_dispatcher;

public:
   virtual ~AgentNode() {m_dispatcher = NULL;}

   void onAccept(NodeVisitor& visitor);

   virtual void update() = 0;

   void setMessageDispatcher(MessageDispatcher& dispatcher)
   {
      m_dispatcher = &dispatcher;
   }

   void resetMessageDispatcher()
   {
      m_dispatcher = NULL;
   }

protected:
   AgentNode(const std::string& name, bool dynamic) 
      : Node(name, dynamic), 
      m_dispatcher(NULL) 
   {}

   inline MessageDispatcher& dispatcher() 
   {
      if (m_dispatcher == NULL)
      {
         throw std::runtime_error("Message dispatcher not present");
      }
      return *m_dispatcher;
   }
};

///////////////////////////////////////////////////////////////////////////////
