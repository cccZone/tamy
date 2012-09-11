/// @file   TamyEditor/GBFloat.h
/// @brief  geometry shader pipeline block representing a float value provider node
#pragma once

#include "core-Renderer/GNFloat.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Geometry shader pipeline block representing a float value provider node.
 */
class GBFloat : public TPipelineBlock< GNFloat, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBFloat() : TPipelineBlock< GNFloat, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBFloat( GNFloat& node );
};

///////////////////////////////////////////////////////////////////////////////
