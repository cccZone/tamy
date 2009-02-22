#pragma once

#include "GraphicalEntity.h"


//////////////////////////////////////////////////////////////////////////////

class GraphicalEntityMock : public GraphicalEntity
{
public:
   GraphicalEntityMock(const std::string& name, 
                       const std::vector<Material*>& materials,
                       const D3DXMATRIX& localMtx) 
         : GraphicalEntity(name, materials, localMtx)
   {
   }

   void render(const D3DXMATRIX& globalMtx, DWORD subset) {}
};

//////////////////////////////////////////////////////////////////////////////
