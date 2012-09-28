#include "RPBFloats2Vec4.h"
#include "QPropertiesView.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBFloats2Vec4 );
   PARENT( GraphBlock );
   PROPERTY( TResourceHandle< RPFloats2Vec4Node >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBFloats2Vec4::RPBFloats2Vec4( RPFloats2Vec4Node& node )
   : TGraphBlock< RPFloats2Vec4Node, RenderingPipelineNode >( node )
{
   setCaption( "Set Vec4" );
}

///////////////////////////////////////////////////////////////////////////////
