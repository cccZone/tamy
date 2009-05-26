#pragma once

#include "core-Renderer\AbstractGraphicalEntity.h"
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
   float m_boundingSphereRadius;

public:
   LeafGraphicalEntity(const std::string& name)
         : AbstractGraphicalEntity(name),
         m_boundingSphereRadius(0)
   {
      D3DXMatrixIdentity(&m_identityMtx);
   }

   virtual ~LeafGraphicalEntity() {}

   void addChild(AbstractGraphicalEntity* child) {delete child;}

   const std::list<AbstractGraphicalEntity*>& getChildren() const {return m_noChildren;}

   const D3DXMATRIX& getLocalMtx() const {return m_identityMtx;}

   /**
   * This method returns the radius of a sphere that binds this graphical entity
   */
   float getBoundingSphereRadius() const {return m_boundingSphereRadius;}

protected:
   void setBoundingSphereRadius(float radius)
   {
      m_boundingSphereRadius = radius;
   }
};

///////////////////////////////////////////////////////////////////////////////
