/// @file   TamyEditor/RPBTexture.h
/// @brief  rendering pipeline block representing a texture provider node
#pragma once

#include "core-Renderer/RPTextureNode.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a texture provider node.
 */
class RPBTexture : public TGraphBlock< RPTextureNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBTexture() : TGraphBlock< RPTextureNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBTexture( RPTextureNode& node );
};

///////////////////////////////////////////////////////////////////////////////
