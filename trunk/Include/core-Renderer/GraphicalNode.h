#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a node that can be rendered on a graphical device.
 * This node instantiates a graphical entity
 */
class GraphicalNode : public AbstractGraphicalNode
{
private:
   GraphicalEntity& m_entity;

public:
   GraphicalNode(const std::string& name, 
                 bool dynamic, 
                 GraphicalEntity& entity, 
                 DWORD subset);

   /**
    * ...self explanatory I think...
    */
   void render();

   /**
    * Returns the graphical representation of the node
    */
   inline GraphicalEntity& getEntity() const {return m_entity;}
};

///////////////////////////////////////////////////////////////////////////////
