#include "RPBBackBuffer.h"
#include "core-Renderer/RPBackBufferNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBBackBuffer, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPBackBufferNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBBackBuffer::RPBBackBuffer( RPBackBufferNode& node )
   : m_nodePtr( &node )
{
   setCaption( "BackBuffer" );

   addSocket().initialize( GBSP_LEFT, "Input" );
   addSocket().initialize( GBSP_RIGHT, "Output" );
}

///////////////////////////////////////////////////////////////////////////////

RPBBackBuffer::~RPBBackBuffer()
{
   delete m_node;
   m_node = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPBBackBuffer::initialize( RenderingPipeline& parentResource )
{
   m_node = new TResourceHandle< RPBackBufferNode >( *m_nodePtr );
}

///////////////////////////////////////////////////////////////////////////////

Object& RPBBackBuffer::getNode() 
{ 
   return m_node->get();
}

///////////////////////////////////////////////////////////////////////////////
