#include "Material.h"
#include "LightReflectingProperties.h"
#include "Texture.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

Material::Material(Texture& emptyTexture)
      : m_lightReflectingProperties(&m_nullLightReflectingProperties),
      m_texture(&emptyTexture)
{
}

///////////////////////////////////////////////////////////////////////////////

bool Material::operator==(const Material& rhs) const
{
   bool equal = (*m_lightReflectingProperties == *(rhs.m_lightReflectingProperties));
   if (equal == false) {return equal;}

   equal &= (*m_texture == *(rhs.m_texture));
   
   return equal;
}

///////////////////////////////////////////////////////////////////////////////

bool Material::operator!=(const Material& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

void Material::setForRendering()
{
   m_lightReflectingProperties->setForRendering();
   m_texture->setForRendering();
}

///////////////////////////////////////////////////////////////////////////////
