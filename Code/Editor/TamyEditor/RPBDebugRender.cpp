#include "RPBDebugRender.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBDebugRender );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< RPDebugRendererNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBDebugRender::RPBDebugRender( RPDebugRendererNode& node )
   : TPipelineBlock< RPDebugRendererNode, RenderingPipelineNode >( node )
{
   setCaption( "Debug scene" );
}

///////////////////////////////////////////////////////////////////////////////
