/// @file   core-Renderer/Adapter.h
/// @brief  a node that allows to append a regular node to a texture output.
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A post-process effect that blurs an image.
 */
class RPAdapterNode : public RenderingPipelineNode
{
   DECLARE_ALLOCATOR( RPAdapterNode, AM_ALIGNED_16 );
   DECLARE_CLASS()

public:
   RPAdapterNode();
};

///////////////////////////////////////////////////////////////////////////////
