#include "core-Renderer\Material.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Texture.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

Material::Material(Texture& emptyTexture, unsigned int index)
      : m_lightReflectingProperties(&m_nullLightReflectingProperties),
      m_texture(&emptyTexture),
      m_index(index)
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

void Material::setLightReflectingProperties(LightReflectingProperties& component) 
{
   m_lightReflectingProperties = &component;
}

///////////////////////////////////////////////////////////////////////////////

bool Material::isTransparent() const
{
   bool isLRPTransparent = m_lightReflectingProperties->isTransparent();
   bool isTexTransparent = m_texture->isTransparent();
   return (m_lightReflectingProperties->isTransparent() || m_texture->isTransparent());
}

///////////////////////////////////////////////////////////////////////////////

void Material::setForRendering()
{
   enableTransparency(isTransparent());

   m_lightReflectingProperties->setForRendering();
   m_texture->setForRendering();
}

///////////////////////////////////////////////////////////////////////////////
