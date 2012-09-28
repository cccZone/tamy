#include "RPBDeferredLighting.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBDeferredLighting );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< RPDeferredLightingNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBDeferredLighting::RPBDeferredLighting( RPDeferredLightingNode& node )
   : TGraphBlock< RPDeferredLightingNode, RenderingPipelineNode >( node )
{
   setCaption( "Lights" );
}

///////////////////////////////////////////////////////////////////////////////
