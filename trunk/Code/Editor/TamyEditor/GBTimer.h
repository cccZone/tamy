/// @file   TamyEditor/GBTimer.h
/// @brief  geometry shader pipeline block representing a time provider node
#pragma once

#include "core-Renderer/GNTimer.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * GeometryShader pipeline block representing a time provider node.
 */
class GBTimer : public TGraphBlock< GNTimer, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBTimer() : TGraphBlock< GNTimer, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBTimer( GNTimer& node );
};

///////////////////////////////////////////////////////////////////////////////
