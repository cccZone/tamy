#include "GraphicalEntity.h"
#include "Material.h"
#include "GraphicalNode.h"
#include <deque>
#include <sstream>
#include "Skeleton.h"

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity::GraphicalEntity(const std::string& name,
                                 const std::vector<Material*>& materials, 
                                 const D3DXMATRIX& localMtx)
      : m_name(name),
      m_materials(materials),
      m_localMtx(localMtx),
      m_skeleton(new Skeleton(AnimationDefinition(), new Node("")))
{
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity::~GraphicalEntity()
{
   delete m_skeleton;
   m_skeleton = NULL;
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
   Node* rootNode = createNode(instanceName, *this, 0);

   std::deque<std::pair<Node*, GraphicalEntity*> > entitiesQueue;
   entitiesQueue.push_back(std::make_pair(rootNode, this));

   while(entitiesQueue.size() > 0)
   {
      Node* currNode = entitiesQueue.front().first;
      GraphicalEntity* currEntity = entitiesQueue.front().second;
      entitiesQueue.pop_front();

      // add all the subsets of the entity - we start counting from
      // the subset one, 'cause THIS very entity will take care of subset 0
      for (unsigned int subsetIdx = 1; subsetIdx < currEntity->getNumSubsets(); ++subsetIdx)
      {
         std::stringstream childNodeName;
         childNodeName << instanceName << "_subset" << subsetIdx;

         Node* newNode = createNode(childNodeName.str(), *currEntity, subsetIdx);

         currNode->addChild(newNode);
      }

      // add children of the hierarchical entity
      for (std::list<GraphicalEntity*>::const_iterator it = currEntity->getChildren().begin();
           it != currEntity->getChildren().end(); ++it)
      {
         Node* newNode = createNode("", **it, 0);
         newNode->setLocalMtx((*it)->getLocalMtx());

         currNode->addChild(newNode);
         entitiesQueue.push_back(std::make_pair(newNode, *it));
      }
   }

   return rootNode;
}

///////////////////////////////////////////////////////////////////////////////

Node* GraphicalEntity::createNode(const std::string& name, 
                                  GraphicalEntity& entity,
                                  DWORD subsetIdx)
{
   Node* node = NULL;
   if (entity.getNumSubsets() == 0)
   {
      node = new Node(name);
   }
   else
   {
      node = new GraphicalNode(name, entity, subsetIdx);
   }
   return node;
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntity::attachSkeleton(Skeleton* skeleton)
{
   if (skeleton == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a Skeleton instance");
   }

   delete m_skeleton;
   m_skeleton = skeleton;
}

///////////////////////////////////////////////////////////////////////////////
