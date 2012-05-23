#include "core-Renderer\SurfaceProperties.h"


///////////////////////////////////////////////////////////////////////////////

SurfaceProperties::SurfaceProperties()
   : m_power(1)
{
}

///////////////////////////////////////////////////////////////////////////////

SurfaceProperties::SurfaceProperties( const Color& ambient, const Color& diffuse, const Color& specular, const Color& emissive, float power )
   : m_ambient( ambient )
   , m_diffuse( diffuse )
   , m_specular( specular )
   , m_emissive( emissive )
   , m_power( power )
{
}

///////////////////////////////////////////////////////////////////////////////

SurfaceProperties::SurfaceProperties( const SurfaceProperties& rhs )
   : m_ambient( rhs.m_ambient )
   , m_diffuse( rhs.m_diffuse )
   , m_specular( rhs.m_specular )
   , m_emissive( rhs.m_emissive )
   , m_power( rhs.m_power )
{
}

///////////////////////////////////////////////////////////////////////////////

bool SurfaceProperties::operator==( const SurfaceProperties& rhs ) const
{
   return ( m_ambient == rhs.m_ambient ) && ( m_diffuse == rhs.m_diffuse ) &&
      ( m_specular == rhs.m_specular ) && ( m_emissive == rhs.m_emissive ) &&
      ( m_power == rhs.m_power );
}

///////////////////////////////////////////////////////////////////////////////
