#pragma once

#include "core\Node.h"
#include "core-Renderer\Renderable.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingTechnique;
class AbstractGraphicalEntity;
struct BoundingSphere;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a node that can be rendered on a graphical device.
 * This node instantiates a graphical entity
 */
class AbstractGraphicalNode : public Node, public Renderable
{
private:
   RenderingTechnique* m_technique;
   DWORD m_subset;

public:
   AbstractGraphicalNode(const std::string& name, 
                         bool dynamic, 
                         RenderingTechnique& technique, 
                         DWORD subset);
   virtual ~AbstractGraphicalNode() {}

   /**
    * The method allows to adjust the bounding sphere's radius
    */
   void setBoundingSphereRadius(float radius);

   /**
    * This method allows to set an arbitrary technique that will be used
    * to render this node's contents
    */
   void setTechnique(RenderingTechnique& technique);

   inline const RenderingTechnique& getTechnique() const {return *m_technique;}
   inline RenderingTechnique& getTechnique() {return *m_technique;}

    /**
    * ...self explanatory I think...
    */
   virtual void render() = 0;

   inline DWORD getSubsetIdx() const {return m_subset;}

protected:
   void onAccept(NodeVisitor& visitor);
};

///////////////////////////////////////////////////////////////////////////////
