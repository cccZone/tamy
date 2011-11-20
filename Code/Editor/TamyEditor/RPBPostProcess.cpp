#include "RPBPostProcess.h"
#include "core-Renderer/RPPostProcessNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBPostProcess, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPPostProcessNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBPostProcess::RPBPostProcess( RPPostProcessNode& node )
   : TRenderingPipelineBlock< RPPostProcessNode >( node )
{
   setCaption( "PostProcess" );
}

///////////////////////////////////////////////////////////////////////////////
