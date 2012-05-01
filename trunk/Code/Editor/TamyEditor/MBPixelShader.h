/// @file   TamyEditor/MBPixelShader.h
/// @brief  material pipeline block representing the material's pixel shader
#pragma once

#include "core-Renderer/MNPixelShader.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"

///////////////////////////////////////////////////////////////////////////////

class MBPixelShader : public TPipelineBlock< MNPixelShader, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBPixelShader() : TPipelineBlock< MNPixelShader, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBPixelShader( MNPixelShader& node );

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
