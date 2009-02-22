#pragma once

#include <d3dx9.h>
#include <list>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Material;
class Node;
class Skeleton;

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
   Skeleton* m_skeleton;

public:
   GraphicalEntity(const std::string& name,
                   const std::vector<Material*>& materials,
                   const D3DXMATRIX& localMtx);
   virtual ~GraphicalEntity();

   const std::string getName() const {return m_name;}

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
    * Attaches a skeleton that will be used to animate 
    * the hierarchy of entities attached to this entity
    */
   void attachSkeleton(Skeleton* skeleton);

   /**
    * Renders the entity on the specified position
    */
   virtual void render(const D3DXMATRIX& globalMtx, DWORD materialIdx) = 0;

private:
   Node* createNode(const std::string& name, 
                    GraphicalEntity& entity, 
                    DWORD subsetIdx);
};

///////////////////////////////////////////////////////////////////////////////
