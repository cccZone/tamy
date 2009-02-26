#include "GraphicalEntity.h"
#include "Material.h"
#include "GraphicalNode.h"
#include <deque>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

GraphicalEntity::GraphicalEntity(const std::string& name,
                                 const std::vector<Material*>& materials, 
                                 const D3DXMATRIX& localMtx)
      : m_name(name),
      m_materials(materials),
      m_localMtx(localMtx)
{
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity::~GraphicalEntity()
{
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntity::setAnimationDefinition(const AnimationDefinition& animationDef)
{
   m_animationData = animationDef;
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

const D3DXMATRIX& GraphicalEntity::getLocalMtx() const
{
   return m_localMtx;
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntity::addChild(GraphicalEntity& child)
{
   m_children.push_back(&child);
}

///////////////////////////////////////////////////////////////////////////////

const std::list<GraphicalEntity*>& GraphicalEntity::getChildren() const
{
   return m_children;
}

///////////////////////////////////////////////////////////////////////////////

Node* GraphicalEntity::instantiate(const std::string& instanceName)
{
   Node* rootNode = new Node(instanceName);

   std::deque<std::pair<Node*, GraphicalEntity*> > entitiesQueue;

   entitiesQueue.push_back(std::make_pair(rootNode, this));

   while(entitiesQueue.size() > 0)
   {
      Node* parentNode = entitiesQueue.front().first;
      GraphicalEntity* currEntity = entitiesQueue.front().second;
      entitiesQueue.pop_front();

      Node* entityRoot = new Node(currEntity->getName());
      parentNode->addChild(entityRoot);
      entityRoot->setLocalMtx(currEntity->getLocalMtx());

      // create graphical node for each subset of current GraphicalEntity
      for (unsigned int subsetIdx = 0; subsetIdx < currEntity->getNumSubsets(); ++subsetIdx)
      {
         std::stringstream childNodeName;
         childNodeName << currEntity->getName() << "_subset" << subsetIdx;
         Node* newNode = new GraphicalNode(childNodeName.str(), *currEntity, subsetIdx);
         entityRoot->addChild(newNode);
      }

      // add children of current GraphicalEntity
      for (std::list<GraphicalEntity*>::const_iterator it = currEntity->getChildren().begin();
           it != currEntity->getChildren().end(); ++it)
      {
         entitiesQueue.push_back(std::make_pair(entityRoot, *it));
      }
   }

   return rootNode;
}

///////////////////////////////////////////////////////////////////////////////

Skeleton* GraphicalEntity::instantiateSkeleton(Node& rootBone)
{
   return new Skeleton(m_animationData, rootBone);
}

///////////////////////////////////////////////////////////////////////////////
