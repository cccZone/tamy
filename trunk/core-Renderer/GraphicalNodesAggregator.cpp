#include "core-Renderer\GraphicalNodesAggregator.h"
#include "core-Renderer\AbstractGraphicalNode.h"


///////////////////////////////////////////////////////////////////////////////

GraphicalNodesAggregator::GraphicalNodesAggregator(const D3DXVECTOR3& cameraPos)
      : m_comparator(cameraPos),
      m_nodes(m_comparator)
{
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalNodesAggregator::visit(AbstractGraphicalNode& node) 
{
   m_nodes.insert(&node);
}

///////////////////////////////////////////////////////////////////////////////

const BatchedNodes& GraphicalNodesAggregator::operator()()
{
   return m_nodes;
}

///////////////////////////////////////////////////////////////////////////////
