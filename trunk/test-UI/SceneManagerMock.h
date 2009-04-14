#pragma once

#include "core-Renderer\SceneManager.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class SceneManagerMock : public SceneManager
{
private:
   std::list<Light*> m_lights;
   Node* m_lastAddedNode;

public:
   SceneManagerMock() : m_lastAddedNode(NULL) {}

   const std::list<Light*>& getLights(int lightLimit)
   {
      return m_lights;
   }

   AbstractGraphicalNodeP* getRegularGraphicalNodes(DWORD& arraySize)
   {
      return NULL;
   }

   AbstractGraphicalNodeP* getTransparentGraphicalNodes(DWORD& arraySize)
   {
      return NULL;
   }

   Node& getLastAddedNode() {return *m_lastAddedNode;}

protected:
   void addToHierarchy(Node* node) {m_lastAddedNode = node;}

   void removeFromHierarchy(Node* node) {}

   void add(Light& light) {}

   void remove(Light& light) {}

   void add(AbstractGraphicalNode& node) {}

   void remove(AbstractGraphicalNode& node) {}
};

///////////////////////////////////////////////////////////////////////////////
