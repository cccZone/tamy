#include "core-Renderer\DirectionalLight.h"
#include "core\BoundingSpace.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( DirectionalLight );
   PARENT( Light );
   PROPERTY_EDIT( "Color", Color, m_color );
   PROPERTY_EDIT( "strength", float, m_strength );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::DirectionalLight( const std::string& name )
   : Light( LT_Directional, name )
   , m_color(1, 1, 1, 1 )
   , m_strength( 1 )
{
   setBoundingVolume( new BoundingSpace() ); 
}

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::DirectionalLight( const DirectionalLight& rhs )
   : Light( rhs )
   , m_color( rhs.m_color )
   , m_strength( rhs.m_strength )
{
   setBoundingVolume( new BoundingSpace() ); 
}

///////////////////////////////////////////////////////////////////////////////

Entity* DirectionalLight::cloneSelf() const
{
   return new DirectionalLight( *this );
}

///////////////////////////////////////////////////////////////////////////////
