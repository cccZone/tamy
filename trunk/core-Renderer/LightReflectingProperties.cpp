#include "core-Renderer\LightReflectingProperties.h"


///////////////////////////////////////////////////////////////////////////////

LightReflectingProperties::LightReflectingProperties()
      : m_power(1)
{
}

///////////////////////////////////////////////////////////////////////////////

LightReflectingProperties::LightReflectingProperties(const LightReflectingProperties& rhs)
      : m_ambient(rhs.m_ambient),
      m_diffuse(rhs.m_diffuse),
      m_specular(rhs.m_specular),
      m_emissive(rhs.m_emissive),
      m_power(rhs.m_power)
{
}

///////////////////////////////////////////////////////////////////////////////

bool LightReflectingProperties::operator==(const LightReflectingProperties& rhs) const
{
   return (m_ambient == rhs.m_ambient) && (m_diffuse == rhs.m_diffuse) &&
          (m_specular == rhs.m_specular) && (m_emissive == rhs.m_emissive) &&
          (m_power == rhs.m_power);
}

///////////////////////////////////////////////////////////////////////////////
