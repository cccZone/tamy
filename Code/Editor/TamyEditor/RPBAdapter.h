/// @file   TamyEditor/RPBStart.h
/// @brief  rendering pipeline block representing a rendering process adapter node
#pragma once

#include "core-Renderer/RPAdapterNode.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a rendering process start node.
 */
class RPBAdapter : public TGraphBlock< RPAdapterNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBAdapter() : TGraphBlock< RPAdapterNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBAdapter( RPAdapterNode& node );
};

///////////////////////////////////////////////////////////////////////////////
