#pragma once

#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

class Material;
class GraphicalEntity;
struct BoundingSphere;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a node that can be rendered on a graphical device.
 * This node instantiates a graphical entity
 */
class AbstractGraphicalNode : public Node
{
protected:
   Material& m_material;
   DWORD m_subset;
   float m_bsRadius;

public:
   AbstractGraphicalNode(const std::string& name, Material& material, DWORD subset);
   virtual ~AbstractGraphicalNode() {}

   /**
    * This method returns a bounding sphere that bounds the node's contents
    * (in world space)
    */
   BoundingSphere getBoundingSphere();

   /**
    * The method allows to adjust the bounding sphere's radius
    */
   void setBoundingSphereRadius(float radius);

   Material& getMaterial() const {return m_material;}

    /**
    * ...self explanatory I think...
    */
   virtual void render() = 0;

   inline DWORD getSubsetIdx() const {return m_subset;}

protected:
   void onAccept(NodeVisitor& visitor);
};

///////////////////////////////////////////////////////////////////////////////
