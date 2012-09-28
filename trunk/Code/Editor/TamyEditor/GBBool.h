/// @file   TamyEditor/GBBool.h
/// @brief  geometry shader pipeline block representing a bool value provider node
#pragma once

#include "core-Renderer/GNBool.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Geometry shader pipeline block representing a bool value provider node.
 */
class GBBool : public TGraphBlock< GNBool, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBBool() : TGraphBlock< GNBool, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBBool( GNBool& node );
};

///////////////////////////////////////////////////////////////////////////////
