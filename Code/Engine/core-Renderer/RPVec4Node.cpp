#include "core-Renderer/RPVec4Node.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPVec4Node );
   PARENT( RenderngPipelineNode );
   PROPERTY_EDIT( "vector", Vector, m_vector );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPVec4Node::RPVec4Node()
   : m_output( new RPVec4Output( "Vec" ) )
{
   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

RPVec4Node::~RPVec4Node()
{
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPVec4Node::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_output = DynamicCast< RPVec4Output >( findOutput( "Vec" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPVec4Node::onUpdate( RenderingPipelineMechanism& host ) const
{
   m_output->setValue( host.data(), m_vector );
}

///////////////////////////////////////////////////////////////////////////////
