#include "MBFloat.h"
#include "core-Renderer/MNFloat.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MBFloat, GraphBlock );
   PROPERTY( TResourceHandle< MNFloat >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MBFloat::MBFloat( MNFloat& node )
   : TPipelineBlock< MNFloat, MaterialNode >( node )
{
   setCaption( "Float" );
}

///////////////////////////////////////////////////////////////////////////////
