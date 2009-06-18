#pragma once

#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

class Material;
class AbstractGraphicalEntity;
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

public:
   AbstractGraphicalNode(const std::string& name, 
                         bool dynamic, 
                         Material& material, 
                         DWORD subset);
   virtual ~AbstractGraphicalNode() {}

   /**
    * The method allows to adjust the bounding sphere's radius
    */
   void setBoundingSphereRadius(float radius);

   Material& getMaterial() const {return m_material;}

   /**
    * The method returns a graphical entity attached to the node
    */
   virtual const AbstractGraphicalEntity& getEntity() const = 0;

    /**
    * ...self explanatory I think...
    */
   virtual void render() = 0;

   inline DWORD getSubsetIdx() const {return m_subset;}

protected:
   void onAccept(NodeVisitor& visitor);
};

///////////////////////////////////////////////////////////////////////////////
