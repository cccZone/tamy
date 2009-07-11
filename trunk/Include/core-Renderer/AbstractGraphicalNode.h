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
private:
   Material* m_material;
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

   /**
    * This method allows to set an arbitrary material that will be used
    * during rendering of the node contents.
    */
   void setMaterial(Material& material);

   inline const Material& getMaterial() const {return *m_material;}
   inline Material& getMaterial() {return *m_material;}

    /**
    * ...self explanatory I think...
    */
   virtual void render() = 0;

   inline DWORD getSubsetIdx() const {return m_subset;}

protected:
   void onAccept(NodeVisitor& visitor);
};

///////////////////////////////////////////////////////////////////////////////
