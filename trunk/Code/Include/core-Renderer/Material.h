#pragma once

/// @file   core-Renderer\Material.h
/// @brief  material's light reflecting properties

#include "core\Color.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents the light reflecting properties of a material.
 */
class Material
{
   DECLARE_RTTI_CLASS

private:
   Color m_ambient;
   Color m_diffuse;
   Color m_specular;
   Color m_emissive;
   float m_power;

public:
   Material();
   Material(const Color& ambient,
      const Color& diffuse,
      const Color& specular,
      const Color& emissive,
      float power);
   Material(const Material& rhs);

   bool operator==(const Material& rhs) const;
   bool operator!=(const Material& rhs) const
   {
      return !(*this == rhs);
   }

   void setAmbientColor(const Color& ambient) {m_ambient = ambient;}
   void setDiffuseColor(const Color& diffuse) {m_diffuse = diffuse;}
   void setSpecularColor(const Color& specular) {m_specular = specular;}
   void setEmissiveColor(const Color& emissive) {m_emissive = emissive;}
   void setPower(float val) {m_power = val;}

   const Color& getAmbientColor() const {return m_ambient;}
   const Color& getDiffuseColor() const {return m_diffuse;}
   const Color& getSpecularColor() const {return m_specular;}
   const Color& getEmissiveColor() const {return m_emissive;}
   float getPower() const {return m_power;}
};

///////////////////////////////////////////////////////////////////////////////
