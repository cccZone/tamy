#include "core-Renderer/RPStartNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPStartNode );
   PARENT( RenderingPipelineNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPStartNode::RPStartNode()
{
   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      defineOutput( new RPVoidOutput( "Output" ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
