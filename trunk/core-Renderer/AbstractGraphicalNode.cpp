#include "AbstractGraphicalNode.h"
#include "NodeVisitor.h"
#include "TNodesVisitor.h"
#include "Material.h"


///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalNode::AbstractGraphicalNode(const std::string& name, Material& material, DWORD subset)
      : Node(name), m_material(material), m_subset(subset)
{
}
///////////////////////////////////////////////////////////////////////////////

void AbstractGraphicalNode::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(TNodesVisitor<AbstractGraphicalNode>);
}

///////////////////////////////////////////////////////////////////////////////

