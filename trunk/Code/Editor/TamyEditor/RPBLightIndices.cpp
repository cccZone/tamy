#include "RPBLightIndices.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBLightIndices );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< RPLightIndicesNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBLightIndices::RPBLightIndices( RPLightIndicesNode& node )
   : TPipelineBlock< RPLightIndicesNode, RenderingPipelineNode >( node )
{
   setCaption( "Index Lights" );
}

///////////////////////////////////////////////////////////////////////////////
