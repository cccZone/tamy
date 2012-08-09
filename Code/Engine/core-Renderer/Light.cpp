#include "core-Renderer/Light.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( Light );
   PARENT( SpatialEntity );
   PROPERTY_EDIT( "Casts shadows", bool, m_castsShadows );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

Light::Light( const std::string& name )
   : SpatialEntity( name )
   , m_castsShadows( false )
{
}

///////////////////////////////////////////////////////////////////////////////
