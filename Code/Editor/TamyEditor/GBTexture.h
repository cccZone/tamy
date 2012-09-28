/// @file   TamyEditor/GBTexture.h
/// @brief  geometry shader pipeline block representing a texture provider node
#pragma once

#include "core-Renderer/GNTexture.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * GeometryShader pipeline block representing a texture provider node.
 */
class GBTexture : public TGraphBlock< GNTexture, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBTexture() : TGraphBlock< GNTexture, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBTexture( GNTexture& node );
};

///////////////////////////////////////////////////////////////////////////////
