#pragma once

#include <d3dx9.h>
#include <list>
#include <vector>
#include "Skeleton.h"


///////////////////////////////////////////////////////////////////////////////

class Material;
class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * This node represents a graphical entity 
 * that can be rendered on a graphical device
 */
class GraphicalEntity
{
private:
   std::string m_name;
   std::vector<Material*> m_materials;
   D3DXMATRIX m_localMtx;
   std::list<GraphicalEntity*> m_children;
   AnimationDefinition m_animationData;

public:
   GraphicalEntity(const std::string& name,
                   const std::vector<Material*>& materials,
                   const D3DXMATRIX& localMtx);
   virtual ~GraphicalEntity();

   const std::string getName() const {return m_name;}

   /**
    * This method allows the user to exchange animation related
    * data for this entity
    */
   void setAnimationDefinition(const AnimationDefinition& animationDef);

   /**
    * Each entity has some material set with which it should be rendered.
    * This method returns a reference to it.
    */
   Material& getMaterial(DWORD subsetIdx) const;

   unsigned int getNumSubsets() const;

   /**
    * The graphical entity is a hierarchical thing.
    * It has a local origin relative to its parent.
    */
   const D3DXMATRIX& getLocalMtx() const;

   void addChild(GraphicalEntity& child);

   const std::list<GraphicalEntity*>& getChildren() const;

   /**
    * This method creates an instance of the GraphicalEntity
    */
   Node* instantiate(const std::string& instanceName);

   /**
    * This factory method creates a skeleton instance
    * from this very entity that will animate
    * the hierarchy of bones passed as the param to this method
    */
   Skeleton* instantiateSkeleton(Node& rootBone);

   /**
    * Renders the entity on the specified position
    */
   virtual void render(const D3DXMATRIX& globalMtx, DWORD materialIdx) = 0;
};

///////////////////////////////////////////////////////////////////////////////
