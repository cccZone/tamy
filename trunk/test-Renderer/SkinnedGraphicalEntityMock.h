#pragma once

#include "SkinnedGraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

class SkinnedGraphicalEntityMock : public SkinnedGraphicalEntity
{
private:
   std::vector<D3DXMATRIX> m_matrices;

public:
   SkinnedGraphicalEntityMock(const std::string& name,
                              const std::vector<SkinBoneDefinition>& skeleton,
                              const std::vector<BonesInfluenceDefinition>& boneSets,
                              const std::vector<Material*>& materials) 
         : SkinnedGraphicalEntity(name, skeleton, boneSets, materials)
   {
   }

   void render(const std::vector<D3DXMATRIX>& matrices, DWORD subset)
   {
      m_matrices = matrices;
   }

   const std::vector<D3DXMATRIX>& getMatricesUsedForRendering() const {return m_matrices;}
};

///////////////////////////////////////////////////////////////////////////////
