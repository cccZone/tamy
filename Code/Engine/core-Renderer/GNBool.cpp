#include "core-Renderer/GNBool.h"
#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/GeometryEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GNBool, GeometryShaderNode )
   PROPERTY_EDIT( "Value", bool, m_value )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

GNBool::GNBool()
   : m_value( true )
   , m_output( new GSBoolOutput( "Value" ) )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

GNBool::~GNBool()
{
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GNBool::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< GSBoolOutput >( findOutput( "Value" ) );
}

///////////////////////////////////////////////////////////////////////////////

void GNBool::preRender( Renderer& renderer, const GeometryEntity& entity ) const
{
   m_output->setValue( entity.data(), m_value );
}

///////////////////////////////////////////////////////////////////////////////
