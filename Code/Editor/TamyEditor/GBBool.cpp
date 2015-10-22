#include "GBBool.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GBBool );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< GNBool >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GBBool::GBBool( GNBool& node )
   : TGraphBlock< GNBool, GeometryShaderNode >( node )
{
   setCaption( "Bool" );
}

///////////////////////////////////////////////////////////////////////////////