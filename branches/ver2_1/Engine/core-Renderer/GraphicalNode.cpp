#include "core-Renderer\GraphicalNode.h"


///////////////////////////////////////////////////////////////////////////////

GraphicalNode::GraphicalNode(const std::string& name, 
                             LeafGraphicalEntity& entity, 
                             DWORD subset)
      : AbstractGraphicalNode(name, entity, subset)
{
   m_renderingMatrices.push_back(D3DXMATRIX());
}

///////////////////////////////////////////////////////////////////////////////

const Array<D3DXMATRIX>& GraphicalNode::getRenderingMatrices()
{
   m_renderingMatrices[0] = getGlobalMtx();
   return m_renderingMatrices;
}

///////////////////////////////////////////////////////////////////////////////
