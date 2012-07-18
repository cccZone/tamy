#include "RPBDeferredLighting.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBDeferredLighting );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< RPDeferredLightingNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBDeferredLighting::RPBDeferredLighting( RPDeferredLightingNode& node )
   : TPipelineBlock< RPDeferredLightingNode, RenderingPipelineNode >( node )
{
   setCaption( "Lights" );
}

///////////////////////////////////////////////////////////////////////////////
