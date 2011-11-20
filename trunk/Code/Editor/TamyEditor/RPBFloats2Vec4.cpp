#include "RPBFloats2Vec4.h"
#include "core-Renderer/RPFloats2Vec4Node.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBFloats2Vec4, RenderingPipelineBlock );
PROPERTY( TResourceHandle< RPFloats2Vec4Node >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBFloats2Vec4::RPBFloats2Vec4( RPFloats2Vec4Node& node )
   : TRenderingPipelineBlock< RPFloats2Vec4Node >( node )
{
   setCaption( "Set Vec4" );
}

///////////////////////////////////////////////////////////////////////////////
