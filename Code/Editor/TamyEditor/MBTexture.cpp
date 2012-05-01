#include "MBTexture.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MBTexture );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< MNTexture >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MBTexture::MBTexture( MNTexture& node )
   : TPipelineBlock< MNTexture, MaterialNode >( node )
{
   setCaption( "Texture" );
}

///////////////////////////////////////////////////////////////////////////////
