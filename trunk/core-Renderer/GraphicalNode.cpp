#include "GraphicalNode.h"
#include "GraphicalNodesAggregator.h"
#include "GraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

GraphicalNode::GraphicalNode(const std::string& name, GraphicalEntity& entity, DWORD subset)
      : Node(name), m_entity(entity), m_subset(subset)
{
}
///////////////////////////////////////////////////////////////////////////////

void GraphicalNode::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(GraphicalNodesAggregator<GraphicalNode>);
}

///////////////////////////////////////////////////////////////////////////////

Material& GraphicalNode::getMaterial()
{
   return m_entity.getMaterial(m_subset);
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalNode::render() 
{
   m_entity.render(getGlobalMtx(), m_subset);
}

///////////////////////////////////////////////////////////////////////////////
