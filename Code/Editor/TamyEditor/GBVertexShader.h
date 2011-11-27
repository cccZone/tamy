/// @file   TamyEditor/GBVertexShader.h
/// @brief  geometry shader pipeline block representing the material's pixel shader
#pragma once

#include "core-Renderer/GNVertexShader.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class GBVertexShader : public TPipelineBlock< GNVertexShader, GeometryShaderNode >
{
   DECLARE_CLASS( GBVertexShader )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBVertexShader() : TPipelineBlock< GNVertexShader, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBVertexShader( GNVertexShader& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }
   virtual QColor getBgColor() const { return QColor( 224, 166, 175 ); }
};

///////////////////////////////////////////////////////////////////////////////
