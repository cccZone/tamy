#pragma once

#include "NullLightReflectingProperties.h"


///////////////////////////////////////////////////////////////////////////////

class LightReflectingProperties;
class Texture;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents the material with which 
 * the GraphicalNodes will be rendered. A material
 * is a composite of material components
 */
class Material
{
private:
   NullLightReflectingProperties m_nullLightReflectingProperties;
   LightReflectingProperties* m_lightReflectingProperties;
   Texture* m_texture;
   unsigned int m_index;

public:
   Material(Texture& emptyTexture, unsigned int index = 0);

   /** 
    * Since materials are used in the rendering strategy as a batching pointer,
    * we want to have a way of sorting through them.
    * Each material can get an id assigned telling its order
    * That way we can use them in the sorting algorithms
    */
   unsigned int getIndex() const {return m_index;}
   bool operator<(const Material& rhs) const {return m_index < rhs.m_index;}

   bool operator==(const Material& rhs) const;
   bool operator!=(const Material& rhs) const;

   void setLightReflectingProperties(LightReflectingProperties& component) 
   {
      m_lightReflectingProperties = &component;
   }

   void setTexture(Texture& texture)
   {
      m_texture = &texture;
   }

   /**
    * This method is engine implementation specific
    * and is supposed to set the material as an active material
    * with which the objects will be rendered
    */
   void setForRendering();
};

///////////////////////////////////////////////////////////////////////////////
