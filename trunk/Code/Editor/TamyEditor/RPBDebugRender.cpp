#include "RPBDebugRender.h"
#include "core-Renderer/RPDebugRendererNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBDebugRender, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPDebugRendererNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBDebugRender::RPBDebugRender( RPDebugRendererNode& node )
   : RenderingPipelineBlock( node )
   , m_nodePtr( &node )
{
   setCaption( "Debug scene" );
}

///////////////////////////////////////////////////////////////////////////////

RPBDebugRender::~RPBDebugRender()
{
   delete m_node;
   m_node = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPBDebugRender::initialize( RenderingPipeline& parentResource )
{
   m_node = new TResourceHandle< RPDebugRendererNode >( *m_nodePtr );
}

///////////////////////////////////////////////////////////////////////////////

Object& RPBDebugRender::getNode() 
{ 
   return m_node->get();
}

///////////////////////////////////////////////////////////////////////////////
