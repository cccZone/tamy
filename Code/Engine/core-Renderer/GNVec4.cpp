#include "core-Renderer/GNVec4.h"
#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/GeometryEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GNVec4 );
   PARENT( GeometryShaderNode );
   PROPERTY_EDIT( "x", float, m_x );
   PROPERTY_EDIT( "y", float, m_y );
   PROPERTY_EDIT( "z", float, m_z );
   PROPERTY_EDIT( "w", float, m_w );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GNVec4::GNVec4()
   : m_x( 0.0f )
   , m_y( 0.0f )
   , m_z( 0.0f )
   , m_w( 0.0f )
   , m_output( new GSVec4Output( "Vec" ) )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

GNVec4::~GNVec4()
{
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GNVec4::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< GSVec4Output >( findOutput( "Vec" ) );
}

///////////////////////////////////////////////////////////////////////////////

void GNVec4::preRender( Renderer& renderer, const GeometryEntity& entity ) const
{
   m_output->setValue( entity.data(), D3DXVECTOR4( m_x, m_y, m_z, m_w ) );
}

///////////////////////////////////////////////////////////////////////////////
