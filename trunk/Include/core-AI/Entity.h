#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a representation of an entity in the world.
 */
class Entity
{
private:
   static unsigned long m_nextId;
   unsigned long m_id;

   D3DXVECTOR3     m_situation;
   D3DXQUATERNION  m_orientation;

public:
   virtual ~Entity() {}

   /**
    * Returns a unique ID assigned to each entity.
    *
    * @return  unique ID number
    */
   unsigned long getID() const;

   /**
    * Allows to change the position an actor is at.
    *
    * @param situation     new position the actor should be placed on
    */
   void setSituation(const D3DXVECTOR3& situation) {m_situation = situation;}

   /**
    * Retrives actor's current world location.
    *
    * @return              position the actor currently occupies
    */
   const D3DXVECTOR3& getSituation() const {return m_situation;}

   /**
    * Allows to change the direction the actor's facing.
    *
    * @param orientation   new direction the actor should be facing
    */
   void setOrientation(const D3DXQUATERNION& orientation) {m_orientation = orientation;}

   /**
    * Retrives the direction the actor's currently facing.
    *
    * @return              actor's current direction
    */
   const D3DXQUATERNION& getOrientation() const {return m_orientation;}

protected:
   Entity();
};

///////////////////////////////////////////////////////////////////////////////
