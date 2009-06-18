#pragma once

#include "core\Array.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core\CollisionTests.h"
#include "core\BoundingVolume.h"


///////////////////////////////////////////////////////////////////////////////

template<typename NodeType>
class LinearNodesStorage
{
private:
   Array<NodeType*>* m_container;

public:
   LinearNodesStorage() 
   {
      m_container = new Array<NodeType*>();
   }

   ~LinearNodesStorage()
   {
      delete m_container;
      m_container = NULL;
   }

   void insert(NodeType& elem)
   {
      m_container->push_back(&elem);
   }

   void remove(NodeType& elem)
   {
      unsigned int index = m_container->find(&elem);
      m_container->remove(index);
   }

   void query(const BoundingVolume& boundingVol, Array<NodeType*>& output) const
   {
      unsigned int elemsCount = m_container->size();
      NodeType* elem = NULL;
      for (unsigned int i = 0; i < elemsCount; ++i)
      {
         elem = (*m_container)[i];
         if (elem->getBoundingVolume().testCollision(boundingVol) == true)
         {
            output.push_back(elem);
         }
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
