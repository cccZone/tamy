#include "GBCamera.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GBCamera );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< GNCamera >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GBCamera::GBCamera( GNCamera& node )
   : TPipelineBlock< GNCamera, GeometryShaderNode >( node )
{
   setCaption( "Camera" );
}

///////////////////////////////////////////////////////////////////////////////
