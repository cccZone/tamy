#include "core-Renderer\PointLight.h"
#include "core\BoundingSphere.h"
#include "core\ResourcesManager.h"



///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( PointLight );
   PARENT( Light );
   PROPERTY_EDIT( "Color", Color, m_color );
   PROPERTY_EDIT( "strength", float, m_strength );
   PROPERTY_EDIT( "attenuation", float, m_attenuation );
   PROPERTY_EDIT( "radius", float, m_radius );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

PointLight::PointLight( const std::string& name )
   : Light( LT_Point, name )
   , m_color(1, 1, 1, 1 )
   , m_attenuation( 1.0f )
   , m_radius( 100.0f )
   , m_strength( 1.0f )
   , m_boundingSphere( NULL )
{
   initialize();
}

///////////////////////////////////////////////////////////////////////////////

PointLight::PointLight( const PointLight& rhs )
   : Light( rhs )
   , m_color( rhs.m_color )
   , m_attenuation( rhs.m_attenuation )
   , m_radius( rhs.m_radius )
   , m_strength( rhs.m_strength )
   , m_boundingSphere( NULL )
{
   initialize();
}


///////////////////////////////////////////////////////////////////////////////

PointLight::~PointLight()
{
}


///////////////////////////////////////////////////////////////////////////////

void PointLight::onObjectLoaded()
{
   __super::onObjectLoaded();

   initialize();
}

///////////////////////////////////////////////////////////////////////////////

void PointLight::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_radius" )
   {
      // we don't use scaling for entity transforms, that's why we need to manually
      // change the bounding sphere radius
      m_boundingSphere->radius = m_radius;
   }
}

///////////////////////////////////////////////////////////////////////////////

void PointLight::initialize()
{
   if ( !m_boundingSphere )
   {
      m_boundingSphere = new BoundingSphere( Vector::ZERO, m_radius );
      setBoundingVolume( m_boundingSphere ); 
   }
   else
   {
      m_boundingSphere->radius = m_radius;
   }
}

///////////////////////////////////////////////////////////////////////////////

Entity* PointLight::cloneSelf() const
{
   return new PointLight( *this );
}

///////////////////////////////////////////////////////////////////////////////
