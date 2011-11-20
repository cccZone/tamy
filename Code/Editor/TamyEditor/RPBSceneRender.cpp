#include "RPBSceneRender.h"
#include "core-Renderer/RPSceneRenderNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBSceneRender, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPSceneRenderNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBSceneRender::RPBSceneRender( RPSceneRenderNode& node )
   : TRenderingPipelineBlock< RPSceneRenderNode >( node )
{
   setCaption( "Scene" );
}

///////////////////////////////////////////////////////////////////////////////
