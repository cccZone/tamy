#include "core-Renderer\AbstractGraphicalNode.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\Material.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalNode::AbstractGraphicalNode(const std::string& name, 
                                             bool dynamic, 
                                             Material& material, 
                                             DWORD subset)
      : Node(name, dynamic),
      m_material(material), 
      m_subset(subset)
{
}
///////////////////////////////////////////////////////////////////////////////

void AbstractGraphicalNode::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(TNodesVisitor<AbstractGraphicalNode>);
}

///////////////////////////////////////////////////////////////////////////////

void AbstractGraphicalNode::setBoundingSphereRadius(float radius)
{
   if (radius < 0) {radius = 0;}
   setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), radius));
}

///////////////////////////////////////////////////////////////////////////////
