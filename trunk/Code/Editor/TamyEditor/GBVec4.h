/// @file   TamyEditor/GBVec4.h
/// @brief  geometry shader pipeline block representing a vector provider node
#pragma once

#include "core-Renderer/GNVec4.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * GeometryShader pipeline block representing a vector provider node.
 */
class GBVec4 : public TPipelineBlock< GNVec4, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBVec4() : TPipelineBlock< GNVec4, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBVec4( GNVec4& node );
};

///////////////////////////////////////////////////////////////////////////////
