/// @file   TamyEditor/RPBLightIndices.h
/// @brief  rendering pipeline block representing a light indexer node
#pragma once

#include "core-Renderer/RPLightIndicesNode.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a light indexer node.
 */
class RPBLightIndices : public TPipelineBlock< RPLightIndicesNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBLightIndices() : TPipelineBlock< RPLightIndicesNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBLightIndices( RPLightIndicesNode& node );
};

///////////////////////////////////////////////////////////////////////////////
