#pragma once

#include <d3dx9.h>
#include "core\Node.h"
#include "core\Array.h"
#include "core-Renderer\RenderableNode.h"


///////////////////////////////////////////////////////////////////////////////

class Material;
class LeafGraphicalEntity;
struct BoundingSphere;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a node that can be rendered on a graphical device.
 * This node instantiates a graphical entity
 */
class AbstractGraphicalNode : public RenderableNode
{
private:
   LeafGraphicalEntity& m_entity;
   DWORD m_subset;

public:
   AbstractGraphicalNode(const std::string& name, 
                         LeafGraphicalEntity& entity,
                         DWORD subset);
   virtual ~AbstractGraphicalNode() {}

   /**
    * The method allows to adjust the bounding sphere's radius
    */
   void setBoundingSphereRadius(float radius);

   void render();

   /** 
    * @Inherited
    */
   virtual const Array<D3DXMATRIX>& getRenderingMatrices() = 0;

   /** 
    * This method returns an index indicating which 'part'
    * of the graphical entity the node describes
    */
   inline DWORD getSubsetIdx() const {return m_subset;}

   const LeafGraphicalEntity& getEntity() const;

   const Array<Triangle*>& getBoundingGeometry() const;
};

///////////////////////////////////////////////////////////////////////////////
