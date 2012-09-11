/// @file   TamyEditor/GBTexture.h
/// @brief  geometry shader pipeline block representing a texture provider node
#pragma once

#include "core-Renderer/GNTexture.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * GeometryShader pipeline block representing a texture provider node.
 */
class GBTexture : public TPipelineBlock< GNTexture, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBTexture() : TPipelineBlock< GNTexture, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBTexture( GNTexture& node );
};

///////////////////////////////////////////////////////////////////////////////
