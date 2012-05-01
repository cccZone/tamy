/// @file   core-Renderer\SurfaceProperties.h
/// @brief  surface's light reflecting properties
#pragma once

#include "core\Color.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents the light reflecting properties of a surface.
 */
class SurfaceProperties
{
   DECLARE_CLASS()

private:
   Color m_ambient;
   Color m_diffuse;
   Color m_specular;
   Color m_emissive;
   float m_power;

public:
   SurfaceProperties();
   SurfaceProperties( const Color& ambient, const Color& diffuse, const Color& specular, const Color& emissive, float power );
   SurfaceProperties( const SurfaceProperties& rhs );

   bool operator==( const SurfaceProperties& rhs ) const;
   inline bool operator!=( const SurfaceProperties& rhs ) const
   {
      return !(*this == rhs);
   }

   void setAmbientColor( const Color& ambient ) { m_ambient = ambient; }
   void setDiffuseColor( const Color& diffuse ) { m_diffuse = diffuse; }
   void setSpecularColor( const Color& specular ) { m_specular = specular; }
   void setEmissiveColor( const Color& emissive ) { m_emissive = emissive; }
   void setPower( float val ) { m_power = val; }

   const Color& getAmbientColor() const { return m_ambient; }
   const Color& getDiffuseColor() const { return m_diffuse; }
   const Color& getSpecularColor() const { return m_specular; }
   const Color& getEmissiveColor() const { return m_emissive; }
   float getPower() const { return m_power; }
};

///////////////////////////////////////////////////////////////////////////////
