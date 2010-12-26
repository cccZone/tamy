#include "RPBSceneRender.h"
#include "core-Renderer/RPSceneRenderNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBSceneRender, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPSceneRenderNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBSceneRender::RPBSceneRender( RPSceneRenderNode& node )
   : m_nodePtr( &node )
{
   setCaption( "Scene" );

   addSocket().initialize( GBSP_LEFT, "Input" );
   addSocket().initialize( GBSP_RIGHT, "Output" );
}

///////////////////////////////////////////////////////////////////////////////

RPBSceneRender::~RPBSceneRender()
{
   delete m_node;
   m_node = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPBSceneRender::initialize( RenderingPipeline& parentResource )
{
   m_node = new TResourceHandle< RPSceneRenderNode >( *m_nodePtr );
}

///////////////////////////////////////////////////////////////////////////////

Object& RPBSceneRender::getNode() 
{ 
   return m_node->get();
}

///////////////////////////////////////////////////////////////////////////////
