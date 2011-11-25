#include "RPBAdapter.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBAdapter, GraphBlock );
   PROPERTY( TResourceHandle< RPAdapterNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBAdapter::RPBAdapter( RPAdapterNode& node )
   : TPipelineBlock< RPAdapterNode, RenderingPipelineNode >( node )
{
   setCaption( "Adapter" );
}

///////////////////////////////////////////////////////////////////////////////