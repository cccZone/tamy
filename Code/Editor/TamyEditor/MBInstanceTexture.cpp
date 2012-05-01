#include "MBInstanceTexture.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MBInstanceTexture );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< MNInstanceTexture >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MBInstanceTexture::MBInstanceTexture( MNInstanceTexture& node )
   : TPipelineBlock< MNInstanceTexture, MaterialNode >( node )
{
   setCaption( "Inst. Texture" );
}

///////////////////////////////////////////////////////////////////////////////
