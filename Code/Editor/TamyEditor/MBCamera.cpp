#include "MBCamera.h"
#include "core-Renderer/MNCamera.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MBCamera, GraphBlock );
   PROPERTY( TResourceHandle< MNCamera >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MBCamera::MBCamera( MNCamera& node )
   : TPipelineBlock< MNCamera, MaterialNode >( node )
{
   setCaption( "Camera" );
}

///////////////////////////////////////////////////////////////////////////////
