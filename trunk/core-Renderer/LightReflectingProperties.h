#pragma once

#include "Color.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents the light reflecting properties of a material with which 
 * the GraphicalNodes will be rendered
 */
class LightReflectingProperties
{
public:
   virtual ~LightReflectingProperties() {}

   virtual bool operator==(const LightReflectingProperties& rhs) const = 0;
   bool operator!=(const LightReflectingProperties& rhs) const
   {
      return !(*this == rhs);
   }

   virtual void setAmbientColor(const Color& ambient) = 0;
   virtual void setDiffuseColor(const Color& diffuse) = 0;
   virtual void setSpecularColor(const Color& specular) = 0;
   virtual void setEmissiveColor(const Color& emissive) = 0;
   virtual void setPower(float val) = 0;

   /**
    * This method is engine implementation specific
    * and is supposed to set the light reflecting properties set 
    * as an active one with which the objects will be rendered
    */
   virtual void setForRendering() = 0;
};

///////////////////////////////////////////////////////////////////////////////
