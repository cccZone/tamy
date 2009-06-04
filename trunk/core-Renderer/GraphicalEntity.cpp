#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

GraphicalEntity::GraphicalEntity(const std::string& name,
                                 const std::vector<Material*>& materials)
      : LeafGraphicalEntity(name),
      m_materials(materials)
{
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity::~GraphicalEntity()
{
}

///////////////////////////////////////////////////////////////////////////////

Material& GraphicalEntity::getMaterial(DWORD subsetIdx) const 
{
   if (subsetIdx >= m_materials.size())
   {
      throw std::out_of_range("There aren't so many subsets defined");
   }
   return *(m_materials.at(subsetIdx));
}

///////////////////////////////////////////////////////////////////////////////

unsigned int GraphicalEntity::getNumSubsets() const
{
   return m_materials.size();
}

///////////////////////////////////////////////////////////////////////////////

Node* GraphicalEntity::instantiate(bool dynamic)
{
   Node* rootNode = new Node(getName(), dynamic);
   rootNode->setLocalMtx(getLocalMtx());

   unsigned int subsetsCount = m_materials.size();
   for (unsigned int subsetIdx = 0; subsetIdx < subsetsCount; ++subsetIdx)
   {
      std::stringstream childNodeName;
      childNodeName << getName() << "_subset" << subsetIdx;
      GraphicalNode* newNode = new GraphicalNode(childNodeName.str(), dynamic, *this, subsetIdx);
      newNode->setBoundingSphereRadius(getBoundingSphereRadius());
      rootNode->addChild(newNode);
   }

   return rootNode;
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntity::accept(GraphicalEntityInstantiator& instantiator)
{
   instantiator.visit(*this);
}

///////////////////////////////////////////////////////////////////////////////
