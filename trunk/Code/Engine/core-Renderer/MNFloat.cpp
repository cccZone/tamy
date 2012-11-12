#include "core-Renderer/MNFloat.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialInstance.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNFloat );
   PARENT( MaterialNode );
   PROPERTY_EDIT( "Value", float, m_value );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MNFloat::MNFloat()
   : m_value( 0.0f )
   , m_output( NULL )
{
   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      m_output = new MSFloatOutput( "Value" );
      defineOutput( m_output );
   }
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

void MNFloat::preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   m_output->setValue( instance.data(), m_value );
}

///////////////////////////////////////////////////////////////////////////////
