#include "core-Renderer/MNFloat.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNFloat, MaterialNode )
   PROPERTY_EDIT( "Value", float, m_value )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

MNFloat::MNFloat()
   : m_value( 0.0f )
   , m_output( new MSFloatOutput( "Value" ) )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

MNFloat::~MNFloat()
{
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNFloat::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< MSFloatOutput >( findOutput( "Value" ) );
}

///////////////////////////////////////////////////////////////////////////////
/*
void RPFloatNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   m_output->setValue( host.data(), m_value );
}

///////////////////////////////////////////////////////////////////////////////
*/