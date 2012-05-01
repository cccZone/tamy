#include "RPBPostProcess.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBPostProcess );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< RPPostProcessNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBPostProcess::RPBPostProcess( RPPostProcessNode& node )
   : TPipelineBlock< RPPostProcessNode, RenderingPipelineNode >( node )
{
   setCaption( "PostProcess" );
}

///////////////////////////////////////////////////////////////////////////////
