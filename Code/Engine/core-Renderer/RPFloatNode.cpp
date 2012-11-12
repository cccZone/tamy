#include "core-Renderer/RPFloatNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPFloatNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Value", float, m_value );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPFloatNode::RPFloatNode()
   : m_value( 0.0f )
   , m_output( NULL )
{
   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      m_output = new RPFloatOutput( "Value" );
      defineOutput( m_output );
   }
}

///////////////////////////////////////////////////////////////////////////////

RPFloatNode::~RPFloatNode()
{
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPFloatNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< RPFloatOutput >( findOutput( "Value" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPFloatNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   m_output->setValue( host.data(), m_value );
}

///////////////////////////////////////////////////////////////////////////////
