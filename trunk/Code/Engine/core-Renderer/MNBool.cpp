#include "core-Renderer/MNBool.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNBool, MaterialNode )
   PROPERTY_EDIT( "Value", bool, m_value )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

MNBool::MNBool()
   : m_value( true )
   , m_output( new MSBoolOutput( "Value" ) )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

MNBool::~MNBool()
{
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNBool::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< MSBoolOutput >( findOutput( "Value" ) );
}

///////////////////////////////////////////////////////////////////////////////

void MNBool::preRender( Renderer& renderer, RuntimeDataBuffer& data ) const
{
   m_output->setValue( data, m_value );
}

///////////////////////////////////////////////////////////////////////////////
