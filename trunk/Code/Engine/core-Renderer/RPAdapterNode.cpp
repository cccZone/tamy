#include "core-Renderer/RPAdapterNode.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/VoidSockets.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPAdapterNode, RenderingPipelineNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPAdapterNode::RPAdapterNode()
{
   defineInput( new RPTextureInput( "Input" ) );
   defineOutput( new RPVoidOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////
