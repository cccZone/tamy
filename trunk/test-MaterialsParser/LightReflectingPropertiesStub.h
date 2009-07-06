#pragma once

#include "core-Renderer\LightReflectingProperties.h"


///////////////////////////////////////////////////////////////////////////////

class LightReflectingPropertiesStub : public LightReflectingProperties
{
private:
   Color m_ambient;
   Color m_diffuse;
   Color m_specular;
   Color m_emissive;
   float m_power;

public:
   LightReflectingPropertiesStub() 
      : m_power(0)
   {}

   bool operator==(const LightReflectingProperties& rhs) const 
   {
      const LightReflectingPropertiesStub* thisRhs = dynamic_cast<const LightReflectingPropertiesStub*>(&rhs);
      if (thisRhs == NULL) {return false;}

      if (m_ambient != thisRhs->m_ambient) {return false;}
      if (m_diffuse != thisRhs->m_diffuse) {return false;}
      if (m_specular != thisRhs->m_specular) {return false;}
      if (m_emissive != thisRhs->m_emissive) {return false;}
      if (m_power != thisRhs->m_power) {return false;}

      return true;
   }

   void setAmbientColor(const Color& ambient) {m_ambient = ambient;}
   void setDiffuseColor(const Color& diffuse) {m_diffuse = diffuse;}
   void setSpecularColor(const Color& specular) {m_specular = specular;}
   void setEmissiveColor(const Color& emissive){m_emissive = emissive;}
   void setPower(float val) {m_power = val;}

   const Color& getDiffuseColor() const {return m_diffuse;}

   void setForRendering() 
   {
   }
};

///////////////////////////////////////////////////////////////////////////////
