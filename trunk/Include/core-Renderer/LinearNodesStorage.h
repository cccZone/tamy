#pragma once

#include "core\Array.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core\CollisionTests.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

class LinearNodesStorage
{
private:
   Array<AbstractGraphicalNode*>* m_container;

public:
   LinearNodesStorage() 
   {
      m_container = new Array<AbstractGraphicalNode*>();
   }

   ~LinearNodesStorage()
   {
      delete m_container;
      m_container = NULL;
   }

   void insert(AbstractGraphicalNode& elem)
   {
      m_container->push_back(&elem);
   }

   void remove(AbstractGraphicalNode& elem)
   {
      unsigned int index = m_container->find(&elem);
      m_container->remove(index);
   }

   template<typename BoundingVolumeType>
   void query(const BoundingVolumeType& boundingVol, 
              Array<AbstractGraphicalNode*>& output) const
   {
      unsigned int elemsCount = m_container->size();
      AbstractGraphicalNode* elem = NULL;
      for (unsigned int i = 0; i < elemsCount; ++i)
      {
         elem = (*m_container)[i];
         if (testCollision(elem->getBoundingSphere(), boundingVol) == true)
         {
            output.push_back(elem);
         }
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
