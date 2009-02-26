#include "GraphicalNode.h"
#include "NodeVisitor.h"
#include "TNodesVisitor.h"
#include "GraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

GraphicalNode::GraphicalNode(const std::string& name, GraphicalEntity& entity, DWORD subset)
      : Node(name), m_entity(entity), m_subset(subset)
{
}
///////////////////////////////////////////////////////////////////////////////

void GraphicalNode::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(TNodesVisitor<GraphicalNode>);
}

///////////////////////////////////////////////////////////////////////////////

Material& GraphicalNode::getMaterial() const
{
   return m_entity.getMaterial(m_subset);
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalNode::render() 
{
   m_entity.render(getGlobalMtx(), m_subset);
}

///////////////////////////////////////////////////////////////////////////////
