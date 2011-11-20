#include "RPBAdapter.h"
#include "core-Renderer/RPAdapterNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBAdapter, RenderingPipelineBlock );
PROPERTY( TResourceHandle< RPAdapterNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBAdapter::RPBAdapter( RPAdapterNode& node )
   : TRenderingPipelineBlock< RPAdapterNode >( node )
{
   setCaption( "Adapter" );
}

///////////////////////////////////////////////////////////////////////////////
