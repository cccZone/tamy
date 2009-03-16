#pragma once

#include "core-Renderer\LightReflectingProperties.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * A class that represents a dummy set of light reflecting properties
 */
class NullLightReflectingProperties : public LightReflectingProperties
{
public:
   bool operator==(const LightReflectingProperties& rhs) const
   {
      const NullLightReflectingProperties* lrp = dynamic_cast<const NullLightReflectingProperties*> (&rhs);
      return (lrp != NULL);
   }

   void setAmbientColor(const Color& ambient) {}
   void setDiffuseColor(const Color& diffuse) {}
   void setSpecularColor(const Color& specular) {}
   void setEmissiveColor(const Color& emissive) {}
   void setPower(float val) {}

   bool isTransparent() const {return false;}

   void setForRendering() {}
};

///////////////////////////////////////////////////////////////////////////////
