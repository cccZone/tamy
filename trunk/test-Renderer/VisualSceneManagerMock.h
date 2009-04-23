#pragma once

#include "core-Renderer\VisualSceneManager.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class VisualSceneManagerMock : public VisualSceneManager
{
private:
   std::list<Light*> m_lights;

public:
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


protected:
   void addToHierarchy(Node* node) {}

   void removeFromHierarchy(Node* node) {}

   void add(Light& light) {}

   void remove(Light& light) {}

   void add(AbstractGraphicalNode& node) {}

   void remove(AbstractGraphicalNode& node) {}
};

///////////////////////////////////////////////////////////////////////////////
