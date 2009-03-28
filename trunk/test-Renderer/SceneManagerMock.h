#pragma once

#include "core-Renderer\SceneManager.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class SceneManagerMock : public SceneManager
{
private:
   std::list<Light*> m_lights;
   int m_callSequence;
   int m_regularNodesQuerySequence;
   int m_transparentNodesQuerySequence;

public:
   SceneManagerMock() 
      : m_callSequence(0),
      m_regularNodesQuerySequence(-1), 
      m_transparentNodesQuerySequence(-1)
   {
   }

   void addNode(Node* node) {}

   const std::list<Light*>& getLights(const Node& cameraNode, 
                                      int lightLimit)
   {
      return m_lights;
   }

   AbstractGraphicalNodeP* getRegularGraphicalNodes(const Node& cameraNode, 
                                             DWORD& arraySize)
   {
      m_regularNodesQuerySequence = m_callSequence++;
      return NULL;
   }

   AbstractGraphicalNodeP* getTransparentGraphicalNodes(const Node& cameraNode,
                                                     DWORD& arraySize)
   {
      m_transparentNodesQuerySequence = m_callSequence++;
      return NULL;
   }

   int getRegularNodesQuerySequence() const
   {
      return m_regularNodesQuerySequence;
   }

   int getTransparentNodesQuerySequence() const
   {
      return m_transparentNodesQuerySequence;
   }
};

///////////////////////////////////////////////////////////////////////////////
