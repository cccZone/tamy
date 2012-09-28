/// @file   TamyEditor/GBVertexShader.h
/// @brief  geometry shader pipeline block representing the material's pixel shader
#pragma once

#include "core-Renderer/GNVertexShader.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class GBVertexShader : public TGraphBlock< GNVertexShader, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBVertexShader() : TGraphBlock< GNVertexShader, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBVertexShader( GNVertexShader& node );
};

///////////////////////////////////////////////////////////////////////////////
