#pragma once

#include "core-Renderer\LeafGraphicalEntity.h"
#include <vector>
#include "core-Renderer\SkinBoneDefinition.h"
#include <map>


///////////////////////////////////////////////////////////////////////////////

class Material;
class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * This node represents a skinned graphical entity 
 * that can be rendered on a graphical device
 */
class SkinnedGraphicalEntity : public LeafGraphicalEntity
{
private:
   std::vector<SkinBoneDefinition> m_skeleton;
   std::vector<BonesInfluenceDefinition> m_boneSets;
   std::vector<Material*> m_materials;

public:
   SkinnedGraphicalEntity(const std::string& name,
                          const std::vector<SkinBoneDefinition>& skeleton,
                          const std::vector<BonesInfluenceDefinition>& boneSets,
                          const std::vector<Material*>& materials);

   virtual ~SkinnedGraphicalEntity();

   /**
    * Each entity has some material set with which it should be rendered.
    * This method returns a reference to it.
    */
   Material& getMaterial(DWORD subsetIdx) const;

   unsigned int getNumSubsets() const;

   /**
    * This method creates an instance of the CompositeGraphicalEntity
    */
   Node* instantiate(Node& boneStructure);

   /**
    * Renders the entity in a skinned way - where each face can have 
    * many global matrices influencing it
    */
   virtual void render(const std::vector<D3DXMATRIX>& matrices, DWORD subset) = 0;

protected:
   void accept(GraphicalEntityInstantiator& instantiator);

private:
   
   struct InstantiationBoneStruct
   {
      Node* boneNode;
      D3DXMATRIX offsetMtx;

      InstantiationBoneStruct(Node* _boneNode, const D3DXMATRIX& _offsetMtx)
            : boneNode(_boneNode), offsetMtx(_offsetMtx)
      {}
   };

   typedef std::map<std::string, InstantiationBoneStruct> InstBonesMap;

   void createNodesMap(Node& boneStructure, InstBonesMap& result) const;
};

///////////////////////////////////////////////////////////////////////////////

