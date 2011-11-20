/// @file   TamyEditor/RPBTexture.h
/// @brief  rendering pipeline block representing a texture provider node
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPTextureNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a texture provider node.
 */
class RPBTexture : public TRenderingPipelineBlock< RPTextureNode >
{
   DECLARE_CLASS( RPBTexture )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBTexture() : TRenderingPipelineBlock< RPTextureNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBTexture( RPTextureNode& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_ROUNDED; }
   virtual QColor getBgColor() const { return QColor( 141, 93, 255 ); }
};

///////////////////////////////////////////////////////////////////////////////
