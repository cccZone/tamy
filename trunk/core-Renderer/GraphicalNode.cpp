#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\GraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

GraphicalNode::GraphicalNode(const std::string& name, GraphicalEntity& entity, DWORD subset)
      : AbstractGraphicalNode(name, entity.getMaterial(subset), subset),
      m_entity(entity)
{
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalNode::render() 
{
   m_entity.render(getGlobalMtx(), m_subset);
}

///////////////////////////////////////////////////////////////////////////////
