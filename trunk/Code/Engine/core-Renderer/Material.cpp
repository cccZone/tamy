#include "core-Renderer\Material.h"


///////////////////////////////////////////////////////////////////////////////

Material::Material()
: m_power(1)
{
}

///////////////////////////////////////////////////////////////////////////////

Material::Material(const Color& ambient,
                   const Color& diffuse,
                   const Color& specular,
                   const Color& emissive,
                   float power)
: m_ambient(ambient)
, m_diffuse(diffuse)
, m_specular(specular)
, m_emissive(emissive)
, m_power(power)
{
}

///////////////////////////////////////////////////////////////////////////////

Material::Material(const Material& rhs)
: m_ambient(rhs.m_ambient)
, m_diffuse(rhs.m_diffuse)
, m_specular(rhs.m_specular)
, m_emissive(rhs.m_emissive)
, m_power(rhs.m_power)
{
}

///////////////////////////////////////////////////////////////////////////////

bool Material::operator==(const Material& rhs) const
{
   return (m_ambient == rhs.m_ambient) && (m_diffuse == rhs.m_diffuse) &&
      (m_specular == rhs.m_specular) && (m_emissive == rhs.m_emissive) &&
      (m_power == rhs.m_power);
}

///////////////////////////////////////////////////////////////////////////////
