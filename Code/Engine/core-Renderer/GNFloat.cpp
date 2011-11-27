#include "core-Renderer/GNFloat.h"
#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/GeometryEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GNFloat, GeometryShaderNode )
   PROPERTY_EDIT( "Value", float, m_value )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

GNFloat::GNFloat()
   : m_value( 0.0f )
   , m_output( new GSFloatOutput( "Value" ) )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

GNFloat::~GNFloat()
{
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GNFloat::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< GSFloatOutput >( findOutput( "Value" ) );
}

///////////////////////////////////////////////////////////////////////////////

void GNFloat::preRender( Renderer& renderer, const GeometryEntity& entity ) const
{
   m_output->setValue( entity.data(), m_value );
}

///////////////////////////////////////////////////////////////////////////////
