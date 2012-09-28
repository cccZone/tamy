/// @file   TamyEditor/RPBFloat.h
/// @brief  rendering pipeline block representing a float value provider node
#pragma once

#include "core-Renderer/RPFloatNode.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a float value provider node.
 */
class RPBFloat : public TGraphBlock< RPFloatNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBFloat() : TGraphBlock< RPFloatNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBFloat( RPFloatNode& node );
};

///////////////////////////////////////////////////////////////////////////////
