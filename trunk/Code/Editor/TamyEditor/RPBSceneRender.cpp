#include "RPBSceneRender.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBSceneRender );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< RPSceneRenderNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBSceneRender::RPBSceneRender( RPSceneRenderNode& node )
   : TPipelineBlock< RPSceneRenderNode, RenderingPipelineNode >( node )
{
   setCaption( "Scene" );
}

///////////////////////////////////////////////////////////////////////////////
