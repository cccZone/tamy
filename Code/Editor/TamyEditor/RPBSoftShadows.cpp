#include "RPBSoftShadows.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBSoftShadows );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< RPSoftShadowsNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBSoftShadows::RPBSoftShadows( RPSoftShadowsNode& node )
   : TPipelineBlock< RPSoftShadowsNode, RenderingPipelineNode >( node )
{
   setCaption( "Shadows" );
}

///////////////////////////////////////////////////////////////////////////////
