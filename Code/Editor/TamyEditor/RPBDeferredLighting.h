/// @file   TamyEditor/RPBDeferredLighting.h
/// @brief  rendering pipeline block representing a lights provider node
#pragma once

#include "core-Renderer/RPDeferredLightingNode.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a lights provider node.
 */
class RPBDeferredLighting : public TGraphBlock< RPDeferredLightingNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBDeferredLighting() : TGraphBlock< RPDeferredLightingNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBDeferredLighting( RPDeferredLightingNode& node );
};

///////////////////////////////////////////////////////////////////////////////
