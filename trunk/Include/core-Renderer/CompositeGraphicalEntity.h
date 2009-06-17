#pragma once

#include "core-Renderer\AbstractGraphicalEntity.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * This node represents a joint that aggregates a bunch of
 * abstract graphical entities
 */
class CompositeGraphicalEntity : public AbstractGraphicalEntity
{
private:
   D3DXMATRIX m_localMtx;
   std::list<AbstractGraphicalEntity*> m_children;

public:
   CompositeGraphicalEntity(const std::string& name,
                           const D3DXMATRIX& localMtx);
   virtual ~CompositeGraphicalEntity();

   void addChild(AbstractGraphicalEntity* child);

   const std::list<AbstractGraphicalEntity*>& getChildren() const;

   const D3DXMATRIX& getLocalMtx() const {return m_localMtx;}

   /**
    * This method creates an instance of the CompositeGraphicalEntity
    */
   Node* instantiate(bool dynamic);

   void getGeometry(Array<Triangle*>& output) const;

protected:
   void accept(GraphicalEntityInstantiator& instantiator);
};

///////////////////////////////////////////////////////////////////////////////
