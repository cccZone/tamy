#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Material.h"
#include "core\BoundingSphere.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class GraphicalNodeMock : public AbstractGraphicalNode
{
private:
   static Material s_material;

public:
   GraphicalNodeMock(bool dynamic = false)
      : AbstractGraphicalNode("graphicalNodeMock", dynamic, s_material, 0)
   {
   }

   void render() {}

   const AbstractGraphicalEntity& getEntity() const 
   {
      return *(reinterpret_cast<const AbstractGraphicalEntity*>(NULL));
   }

   const BoundingSphere& getBoundingSphere()
   {
      return dynamic_cast<const BoundingSphere&> (getBoundingVolume());
   }
};

///////////////////////////////////////////////////////////////////////////////

namespace GraphicalEffectTests
{

   class GraphicalNodeMock : public AbstractGraphicalNode
   {
   private:
      static Material s_material;

      std::vector<std::string>& m_callSequenceVec;

   public:
      GraphicalNodeMock(std::vector<std::string>& callSequenceVec)
         : AbstractGraphicalNode("graphicalNodeMock", false, s_material, 0),
         m_callSequenceVec(callSequenceVec)
      {
      }

      void render() 
      {
         m_callSequenceVec.push_back("GraphicalNode::render");
      }

      const AbstractGraphicalEntity& getEntity() const 
      {
         return *(reinterpret_cast<const AbstractGraphicalEntity*>(NULL));
      }

      const BoundingSphere& getBoundingSphere()
      {
         return dynamic_cast<const BoundingSphere&> (getBoundingVolume());
      }
   };

}

///////////////////////////////////////////////////////////////////////////////
