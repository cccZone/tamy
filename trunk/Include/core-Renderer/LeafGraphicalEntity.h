#pragma once

#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core\Triangle.h"
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

   Array<Triangle*>* m_geometry;

public:
   LeafGraphicalEntity(const std::string& name)
         : AbstractGraphicalEntity(name),
         m_boundingSphereRadius(0),
         m_geometry(new Array<Triangle*>())
   {
      D3DXMatrixIdentity(&m_identityMtx);
   }

   virtual ~LeafGraphicalEntity() 
   {
      unsigned int trisCount = m_geometry->size();
      for (unsigned int i = 0; i < trisCount; ++i)
      {
         delete (*m_geometry)[i];
      }
      delete m_geometry; 
      m_geometry = NULL;
   }

   void addChild(AbstractGraphicalEntity* child) {delete child;}

   const std::list<AbstractGraphicalEntity*>& getChildren() const {return m_noChildren;}

   const D3DXMATRIX& getLocalMtx() const {return m_identityMtx;}

   /**
   * This method returns the radius of a sphere that binds this graphical entity
   */
   float getBoundingSphereRadius() const {return m_boundingSphereRadius;}

   const Array<Triangle*>&  getGeometry() const {return *m_geometry;}

protected:
   void setBoundingSphereRadius(float radius)
   {
      m_boundingSphereRadius = radius;
   }

   void addTriangle(const Triangle& tri) 
   {
      m_geometry->push_back(new Triangle(tri));
   }
};

///////////////////////////////////////////////////////////////////////////////
