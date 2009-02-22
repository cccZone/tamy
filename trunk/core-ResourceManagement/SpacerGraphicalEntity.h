#pragma once

#include "GraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

class SpacerGraphicalEntity : public GraphicalEntity
{
public:
   SpacerGraphicalEntity(const std::string& name,
                         const D3DXMATRIX& localMtx)
         : GraphicalEntity(name, std::vector<Material*>(), localMtx)
   {
   }

   void render(const D3DXMATRIX& globalMtx, DWORD materialIdx)
   {
   }
};

///////////////////////////////////////////////////////////////////////////////
