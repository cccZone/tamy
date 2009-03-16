#pragma once

#include "core-Renderer\GraphicalEntity.h"


//////////////////////////////////////////////////////////////////////////////

class GraphicalEntityMock : public GraphicalEntity
{
public:
   GraphicalEntityMock(const std::string& name, 
                       const std::vector<Material*>& materials) 
         : GraphicalEntity(name, materials)
   {
   }

   void render(const D3DXMATRIX& globalMtx, DWORD subset) {}
};

//////////////////////////////////////////////////////////////////////////////
