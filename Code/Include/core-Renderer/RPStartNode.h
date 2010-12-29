/// @file   core-Renderer/RPStartNode.h
/// @brief  a rendering pipeline node from which the rendering process starts.
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node from which the rendering process starts.
 */
class RPStartNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPStartNode )

public:
   RPStartNode();
};

///////////////////////////////////////////////////////////////////////////////
