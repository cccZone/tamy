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

public:
   Material(Texture& emptyTexture);

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
