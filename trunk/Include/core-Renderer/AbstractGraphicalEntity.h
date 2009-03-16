#pragma once

#include <d3dx9.h>
#include "core-Renderer\Skeleton.h"


///////////////////////////////////////////////////////////////////////////////

class Node;
class GraphicalEntityInstantiator;

///////////////////////////////////////////////////////////////////////////////

/**
 * This node represents a graphical entity 
 * that can be rendered on a graphical device
 */
class AbstractGraphicalEntity
{
private:
   std::string m_name;
   AnimationDefinition m_animationData;

public:
   AbstractGraphicalEntity(const std::string& name);
   virtual ~AbstractGraphicalEntity();

   const std::string getName() const {return m_name;}

   /**
    * This method allows the user to exchange animation related
    * data for this entity
    */
   void setAnimationDefinition(const AnimationDefinition& animationDef);

   /**
    * The graphical entity is a hierarchical thing.
    * It has a local origin relative to its parent.
    */
   virtual const D3DXMATRIX& getLocalMtx() const = 0;

   /**
    * This method supports the composite nature of a raphical entity.
    * Please implement it in the implementations as applicable to their purpose
    */
   virtual void addChild(AbstractGraphicalEntity* child) = 0;

   /**
    * This method supports the composite nature of a graphical entity.
    * Please implement it in the implementations as applicable to their purpose
    */
   virtual const std::list<AbstractGraphicalEntity*>& getChildren() const = 0;

   /**
    * This factory method creates a skeleton instance
    * from this very entity that will animate
    * the hierarchy of bones passed as the param to this method
    */
   Skeleton* instantiateSkeleton(Node& rootBone);

protected:
   friend class GraphicalEntityInstantiator;

   /**
    * This method creates an instance of an entity.
    */
   virtual void accept(GraphicalEntityInstantiator& instantiator) = 0;
};

///////////////////////////////////////////////////////////////////////////////
