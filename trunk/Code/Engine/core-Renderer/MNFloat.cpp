#include "core-Renderer/MNFloat.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNFloat );
   PARENT( MaterialNode );
   PROPERTY_EDIT( "Value", float, m_value );
END_OBJECT();

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

void MNFloat::preRender( Renderer& renderer, const MaterialEntity& entity ) const
{
   m_output->setValue( entity.data(), m_value );
}

///////////////////////////////////////////////////////////////////////////////
