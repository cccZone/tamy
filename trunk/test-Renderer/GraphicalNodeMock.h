#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Material.h"
#include "LightReflectingPropertiesStub.h"
#include "MaterialOperationImplementationMock.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalNodeMock : public AbstractGraphicalNode
{
private:
   static MaterialOperationImplementationMock s_matOpImpl;
   static LightReflectingPropertiesStub s_lrp;
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
