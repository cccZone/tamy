#include "core-Renderer/RPVec4Node.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPVec4Node, RenderingPipelineNode )
   PROPERTY_EDIT( "x", float, m_x )
   PROPERTY_EDIT( "y", float, m_y )
   PROPERTY_EDIT( "z", float, m_z )
   PROPERTY_EDIT( "w", float, m_w )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPVec4Node::RPVec4Node()
   : m_x( 0.0f )
   , m_y( 0.0f )
   , m_z( 0.0f )
   , m_w( 0.0f )
   , m_output( new RPVec4Output( "Vec" ) )
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
   m_output->setValue( host.data(), D3DXVECTOR4( m_x, m_y, m_z, m_w ) );
}

///////////////////////////////////////////////////////////////////////////////
