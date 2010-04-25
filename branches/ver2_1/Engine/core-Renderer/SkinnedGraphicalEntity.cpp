#include "core-Renderer\SkinnedGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\SkinnedGraphicalNode.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include <deque>
#include <sstream>
#include <map>
#include <stdexcept>
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

SkinnedGraphicalEntity::SkinnedGraphicalEntity(const std::string& name,
                                               const std::vector<SkinBoneDefinition>& skeleton,
                                               const std::vector<BonesInfluenceDefinition>& boneSets,
                                               const std::vector<Material*>& materials)
      : LeafGraphicalEntity(name),
      m_skeleton(skeleton),
      m_boneSets(boneSets),
      m_materials(materials)
{
   // the number of bone influences must match the number of materials passed
   if (boneSets.size() != materials.size())
   {
      throw std::invalid_argument("Invalid attributes definition - number of bone influences doesn't\
                                  match the number of rendering materials per attribute");
   }
}

///////////////////////////////////////////////////////////////////////////////

SkinnedGraphicalEntity::~SkinnedGraphicalEntity()
{
}

///////////////////////////////////////////////////////////////////////////////

Material& SkinnedGraphicalEntity::getMaterial(DWORD subsetIdx) const 
{
   if (subsetIdx >= m_materials.size())
   {
      throw std::out_of_range("There aren't so many subsets defined");
   }
   return *(m_materials.at(subsetIdx));
}

///////////////////////////////////////////////////////////////////////////////

unsigned int SkinnedGraphicalEntity::getNumSubsets() const
{
   return m_boneSets.size();
}

///////////////////////////////////////////////////////////////////////////////

struct InstantiationBoneStruct
{
   Node* boneNode;
   D3DXMATRIX offsetMtx;

   InstantiationBoneStruct(Node* _boneNode, const D3DXMATRIX& _offsetMtx)
         : boneNode(_boneNode), offsetMtx(_offsetMtx)
   {}
};

Node* SkinnedGraphicalEntity::instantiate(Node& boneStructure)
{
   Node* rootNode = new Node(getName());
   rootNode->setLocalMtx(getLocalMtx());
   
   InstBonesMap bonesForAttribs;
   createNodesMap(boneStructure, bonesForAttribs);

   // create the skinned nodes that will take care
   // of rendering this entity
   unsigned int subsetsCount = m_boneSets.size();
   for (unsigned int subsetIdx = 0; subsetIdx < subsetsCount; ++subsetIdx)
   {
      std::stringstream childNodeName;
      childNodeName << getName() << "_subset" << subsetIdx;

      SkinnedGraphicalNode::BonesDef boneDef;
      unsigned int boneDefsCount = m_boneSets.at(subsetIdx).size();
      for (unsigned int attribBoneDefIdx = 0; attribBoneDefIdx < boneDefsCount; ++attribBoneDefIdx)
      {
         const std::string& boneName = m_boneSets.at(subsetIdx).at(attribBoneDefIdx);
         InstBonesMap::const_iterator defIt = bonesForAttribs.find(boneName);

         if (defIt == bonesForAttribs.end())
         {
            throw std::runtime_error(std::string("Definition of bone '") + boneName + std::string("' not found"));
         }

         boneDef.push_back(std::make_pair(defIt->second.boneNode, defIt->second.offsetMtx));
      }

      SkinnedGraphicalNode* newNode = new SkinnedGraphicalNode(childNodeName.str(), *this, subsetIdx, boneDef);
      newNode->setBoundingSphereRadius(getBoundingSphereRadius());
      rootNode->addChild(newNode);
   }

   return rootNode;
}

///////////////////////////////////////////////////////////////////////////////

void SkinnedGraphicalEntity::createNodesMap(Node& boneStructure, 
                                            InstBonesMap& result) const
{
   for (std::vector<SkinBoneDefinition>::const_iterator it = m_skeleton.begin();
        it != m_skeleton.end(); ++it)
   {
      result.insert(std::make_pair(it->name, InstantiationBoneStruct(NULL, it->boneOffset)));
   }

   // linearize the bones skeletal structure
   std::deque<Node*> bonesQueue;
   bonesQueue.push_back(&boneStructure);
   while(bonesQueue.size() > 0)
   {
      Node& currNode = *(bonesQueue.front());
      bonesQueue.pop_front();

      // store the result in the temporary struct for the future use
      // of SkinnedGraphicalNodes instantiation
      InstBonesMap::iterator it = result.find(currNode.getName());
      if (it != result.end())
      {
         it->second.boneNode = &currNode;
      }

      // put the children of the bone in the queue for further processing
      const std::list<Node*>& children = currNode.getChildren();
      for (std::list<Node*>::const_iterator it = children.begin();
           it != children.end(); ++it)
      {
         bonesQueue.push_back(*it);
      }
   }

   // check the results to see if all the bones have the corresponding nodes assigned
   for (InstBonesMap::iterator it = result.begin();
        it != result.end(); ++it)
   {
      if (it->second.boneNode == NULL)
      {
         throw std::runtime_error(std::string("There's no node for bone '") + it->first + std::string("'"));
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SkinnedGraphicalEntity::accept(GraphicalEntityInstantiator& instantiator)
{
   instantiator.visit(*this);
}

///////////////////////////////////////////////////////////////////////////////
