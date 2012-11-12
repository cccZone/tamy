#include "core-Renderer/MNBool.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialInstance.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNBool );
   PARENT( MaterialNode );
   PROPERTY_EDIT( "Value", bool, m_value )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

MNBool::MNBool()
   : m_value( true )
   , m_output( NULL )
{
   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      m_output = new MSBoolOutput( "Value" );
      defineOutput( m_output );
   }
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

void MNBool::preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   m_output->setValue( instance.data(), m_value );
}

///////////////////////////////////////////////////////////////////////////////
