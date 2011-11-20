#include "RPBDebugRender.h"
#include "core-Renderer/RPDebugRendererNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBDebugRender, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPDebugRendererNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBDebugRender::RPBDebugRender( RPDebugRendererNode& node )
   : TRenderingPipelineBlock< RPDebugRendererNode >( node )
{
   setCaption( "Debug scene" );
}

///////////////////////////////////////////////////////////////////////////////
