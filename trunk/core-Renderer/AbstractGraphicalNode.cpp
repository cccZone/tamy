#include "core-Renderer\AbstractGraphicalNode.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\LeafGraphicalEntity.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalNode::AbstractGraphicalNode(const std::string& name, 
                                             LeafGraphicalEntity& entity,
                                             DWORD subset)
      : RenderableNode(name, entity.getMaterial(subset)),
      m_entity(entity),
      m_subset(subset)
{
}

///////////////////////////////////////////////////////////////////////////////

void AbstractGraphicalNode::setBoundingSphereRadius(float radius)
{
   if (radius < 0) {radius = 0;}
   setBoundingVolume(new BoundingSphere(D3DXVECTOR3(0, 0, 0), radius));
}

///////////////////////////////////////////////////////////////////////////////

void AbstractGraphicalNode::render()
{
   m_entity.render(m_subset);
}

///////////////////////////////////////////////////////////////////////////////

const Array<Triangle*>& AbstractGraphicalNode::getBoundingGeometry() const
{
   return m_entity.getGeometry();
}

///////////////////////////////////////////////////////////////////////////////

const LeafGraphicalEntity& AbstractGraphicalNode::getEntity() const 
{
   return m_entity;
}

///////////////////////////////////////////////////////////////////////////////
