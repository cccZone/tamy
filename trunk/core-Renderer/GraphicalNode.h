#pragma once

#include "Node.h"


///////////////////////////////////////////////////////////////////////////////

class Material;
class GraphicalEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a node that can be rendered on a graphical device.
 * This node instantiates a graphical entity
 */
class GraphicalNode : public Node
{
private:
   GraphicalEntity& m_entity;
   DWORD m_subset;

public:
   GraphicalNode(const std::string& name, GraphicalEntity& entity, DWORD subset);

   Material& getMaterial();

   /**
    * ...self explanatory I think...
    */
   void render();

   /**
    * Returns the graphical representation of the node
    */
   GraphicalEntity& getEntity() const {return m_entity;}

protected:
   void onAccept(NodeVisitor& visitor);
};

///////////////////////////////////////////////////////////////////////////////
