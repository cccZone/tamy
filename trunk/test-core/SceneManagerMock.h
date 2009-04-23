#pragma once

#include "core\AbstractSceneManager.h"
#include "core\TNodesVisitor.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

template<typename NodeType>
class SceneManagerMock : public AbstractSceneManager,
                         public TNodesVisitor<NodeType>
{
private:
   std::list<NodeType*> m_nodes;
   bool m_adding;

public:
   void visit(NodeType& node)
   {
      if (m_adding)
      {
         m_nodes.push_back(&node);
      }
      else
      {
         for (std::list<NodeType*>::iterator it = m_nodes.begin();
              it != m_nodes.end(); ++it)
         {
            if (*it == &node)
            {
               m_nodes.erase(it);
               break;
            }
         }
      }
   }

   unsigned int getNodesCount() const {return m_nodes.size();}

   void addNode(Node* node)
   {
      m_adding = true;
      node->accept(*this);
   }

   void removeNode(Node& node)
   {
      m_adding = false;
      node.accept(*this);
   }
};

///////////////////////////////////////////////////////////////////////////////
