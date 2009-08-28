#include "core-Renderer\SkinnedGraphicalNode.h"


///////////////////////////////////////////////////////////////////////////////

SkinnedGraphicalNode::SkinnedGraphicalNode(const std::string& name,
                                           LeafGraphicalEntity& entity,
                                           DWORD subset,
                                           const std::vector<std::pair<Node*, D3DXMATRIX> >& bones) 
      : AbstractGraphicalNode(name, entity, subset), 
      m_bones(bones),
      m_renderingMatrices(m_bones.size())
{
   unsigned int bonesCount = m_bones.size();
   for (unsigned int i = 0; i < bonesCount; ++i)
   {
      m_renderingMatrices.push_back(D3DXMATRIX());
   }
}

///////////////////////////////////////////////////////////////////////////////

const Array<D3DXMATRIX>& SkinnedGraphicalNode::getRenderingMatrices()
{
   unsigned int bonesCount = m_bones.size();
   for (unsigned int i = 0; i < bonesCount; ++i)
   {
      const D3DXMATRIX& offsetMtx = m_bones[i].second;
      const D3DXMATRIX& globalMtx = m_bones[i].first->getGlobalMtx();
      D3DXMatrixMultiply(&(m_renderingMatrices[i]), &offsetMtx, &globalMtx);
   }

   return m_renderingMatrices;
}

///////////////////////////////////////////////////////////////////////////////
