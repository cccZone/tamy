#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"
#include "GraphicalEntityMock.h"
#include "core\BoundingSphere.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class GraphicalNodeMock : public AbstractGraphicalNode
{
private:
   static std::vector<Material*> materials;
   static GraphicalEntityMock m_entity;
   Array<D3DXMATRIX> m_matrices;

public:
   GraphicalNodeMock()
      : AbstractGraphicalNode("graphicalNodeMock", m_entity, 0)
   {
   }

   const Array<D3DXMATRIX>& getRenderingMatrices() {return m_matrices;}

   const BoundingSphere& getBoundingSphere()
   {
      return dynamic_cast<const BoundingSphere&> (getBoundingVolume());
   }
};

///////////////////////////////////////////////////////////////////////////////
