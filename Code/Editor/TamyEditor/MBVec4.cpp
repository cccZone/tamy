#include "MBVec4.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MBVec4 );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< MNVec4 >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MBVec4::MBVec4( MNVec4& node )
   : TGraphBlock< MNVec4, MaterialNode >( node )
{
   setCaption( "Vec4" );
}

///////////////////////////////////////////////////////////////////////////////
