#pragma once

#include "core-Renderer\SkinnedGraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

class SkinnedGraphicalEntityMock : public SkinnedGraphicalEntity
{
public:
   SkinnedGraphicalEntityMock(const std::string& name,
                              const std::vector<SkinBoneDefinition>& skeleton,
                              const std::vector<BonesInfluenceDefinition>& boneSets,
                              const std::vector<Material*>& materials) 
         : SkinnedGraphicalEntity(name, skeleton, boneSets, materials)
   {
   }

   void render(DWORD subset)
   {
   }
};

///////////////////////////////////////////////////////////////////////////////
