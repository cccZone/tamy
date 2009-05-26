#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Material.h"
#include "LightReflectingPropertiesStub.h"
#include "MaterialOperationImplementationMock.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalNodeMock : public AbstractGraphicalNode
{
private:
   static MaterialOperationImplementationMock s_matOpImpl;
   static LightReflectingPropertiesStub s_lrp;
   static Material s_material;

public:
   GraphicalNodeMock()
      : AbstractGraphicalNode("graphicalNodeMock", s_material, 0)
   {
   }

   void render() {}
};

///////////////////////////////////////////////////////////////////////////////
