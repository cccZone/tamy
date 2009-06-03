#pragma once

#include "core\AbstractSceneManager.h"
#include "core\TNodesVisitor.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

template<typename NodeType>
class SceneManagerMock : public AbstractSceneManager,
                         public SceneAspectManager<NodeType>
{
private:
   std::list<NodeType*> m_nodes;

public:
   SceneManagerMock() 
   {
      REGISTER_SCENE_ASPECT(NodeType);
   }

   unsigned int getNodesCount() const {return m_nodes.size();}

   void add(NodeType& node)
   {
      m_nodes.push_back(&node);
   }

   void remove(NodeType& node)
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

};

///////////////////////////////////////////////////////////////////////////////
