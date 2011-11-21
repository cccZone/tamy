#include "RPBCamera.h"
#include "core-Renderer/RPCameraNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBCamera, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPCameraNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBCamera::RPBCamera( RPCameraNode& node )
   : TRenderingPipelineBlock< RPCameraNode >( node )
{
   setCaption( "Camera" );
}

///////////////////////////////////////////////////////////////////////////////