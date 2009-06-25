#pragma once

#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntityMock : public AbstractGraphicalEntity
{
private:
   D3DXMATRIX m_identityMtx;
   std::list<AbstractGraphicalEntity*> m_noChildren;
   Array<Triangle*> m_noGeometry;

public:
   GraphicalEntityMock() : AbstractGraphicalEntity("mockEntity")
   {
      D3DXMatrixIdentity(&m_identityMtx);
   }

   const D3DXMATRIX& getLocalMtx() const {return m_identityMtx;}

   void addChild(AbstractGraphicalEntity* child) {}

   const std::list<AbstractGraphicalEntity*>& getChildren() const {return m_noChildren;}

   const Array<Triangle*>& getGeometry() const {return m_noGeometry;}

protected:
   void accept(GraphicalEntityInstantiator& instantiator)
   {      
   }
};

///////////////////////////////////////////////////////////////////////////////
