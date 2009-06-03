#pragma once

#include "core-Renderer\AbstractGraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntityMock : public AbstractGraphicalEntity
{
private:
   D3DXMATRIX m_identityMtx;
   std::list<AbstractGraphicalEntity*> m_noChildren;

public:
   GraphicalEntityMock() : AbstractGraphicalEntity("mockEntity")
   {
      D3DXMatrixIdentity(&m_identityMtx);
   }

   const D3DXMATRIX& getLocalMtx() const {return m_identityMtx;}

   void addChild(AbstractGraphicalEntity* child) {}

   const std::list<AbstractGraphicalEntity*>& getChildren() const {return m_noChildren;}

protected:
   void accept(GraphicalEntityInstantiator& instantiator)
   {      
   }
};

///////////////////////////////////////////////////////////////////////////////
