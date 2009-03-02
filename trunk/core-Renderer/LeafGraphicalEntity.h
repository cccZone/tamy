#pragma once

#include "AbstractGraphicalEntity.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * This abstract class represents a leaf of a graphical entity.
 * It can't have any children - but how it's instantiated - that's
 * left up to the implementation's discretion
 */
class LeafGraphicalEntity : public AbstractGraphicalEntity
{
private:
   D3DXMATRIX m_identityMtx;
   std::list<AbstractGraphicalEntity*> m_noChildren;

public:
   LeafGraphicalEntity(const std::string& name)
         : AbstractGraphicalEntity(name) 
   {
      D3DXMatrixIdentity(&m_identityMtx);
   }

   virtual ~LeafGraphicalEntity() {}

   void addChild(AbstractGraphicalEntity* child) {delete child;}

   const std::list<AbstractGraphicalEntity*>& getChildren() const {return m_noChildren;}

   const D3DXMATRIX& getLocalMtx() const {return m_identityMtx;}
};

///////////////////////////////////////////////////////////////////////////////
