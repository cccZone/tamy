#include "RenderingPipelineBlock.h"
#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RenderingPipelineBlock, GraphBlock )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineBlock::onConnectionCreated( GraphBlockConnection& connection )
{
   RenderingPipelineNode& sourceNode = dynamic_cast< RenderingPipelineNode& >( getNode() );
   RenderingPipelineNode& destinationNode = dynamic_cast< RenderingPipelineNode& >( connection.getDestination().getParentBlock().getNode() );
   sourceNode.addNeighbor( destinationNode );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineBlock::onConnectionRemoved( GraphBlockConnection& connection )
{
   RenderingPipelineNode& sourceNode = dynamic_cast< RenderingPipelineNode& >( getNode() );
   RenderingPipelineNode& destinationNode = dynamic_cast< RenderingPipelineNode& >( connection.getDestination().getParentBlock().getNode() );
   sourceNode.removeNeighbor( destinationNode );
}

///////////////////////////////////////////////////////////////////////////////
