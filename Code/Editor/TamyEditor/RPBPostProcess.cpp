#include "RPBPostProcess.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBPostProcess );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< RPPostProcessNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBPostProcess::RPBPostProcess( RPPostProcessNode& node )
   : TGraphBlock< RPPostProcessNode, RenderingPipelineNode >( node )
{
   setCaption( "PostProcess" );
}

///////////////////////////////////////////////////////////////////////////////
