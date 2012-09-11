/// @file   TamyEditor/RPBStart.h
/// @brief  rendering pipeline block representing a rendering process start node
#pragma once

#include "core-Renderer/RPStartNode.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a rendering process start node.
 */
class RPBStart : public TPipelineBlock< RPStartNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBStart() : TPipelineBlock< RPStartNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBStart( RPStartNode& node );
};

///////////////////////////////////////////////////////////////////////////////
