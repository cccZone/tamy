#include "core-Renderer\AbstractGraphicalNode.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\Material.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalNode::AbstractGraphicalNode(const std::string& name, Material& material, DWORD subset)
      : Node(name), m_material(material), m_subset(subset), m_bsRadius(0)
{
}
///////////////////////////////////////////////////////////////////////////////

void AbstractGraphicalNode::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(TNodesVisitor<AbstractGraphicalNode>);
}

///////////////////////////////////////////////////////////////////////////////

BoundingSphere AbstractGraphicalNode::getBoundingSphere()
{
   D3DXMATRIX globalMtx = getGlobalMtx();
   return BoundingSphere(D3DXVECTOR3(globalMtx._41, globalMtx._42, globalMtx._43), m_bsRadius);
}

///////////////////////////////////////////////////////////////////////////////

void AbstractGraphicalNode::setBoundingSphereRadius(float radius)
{
   if (radius < 0)
   {
      m_bsRadius = 0;
   }
   else
   {
      m_bsRadius = radius;
   }
}

///////////////////////////////////////////////////////////////////////////////
