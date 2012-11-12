#include "core-Renderer/RPAdapterNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPAdapterNode );
   PARENT( RenderingPipelineNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPAdapterNode::RPAdapterNode()
{
   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      defineInput( new RPTextureInput( "Input" ) );
      defineOutput( new RPVoidOutput( "Output" ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
