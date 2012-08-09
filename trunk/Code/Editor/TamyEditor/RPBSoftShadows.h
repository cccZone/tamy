/// @file   TamyEditor/RPBSoftShadows.h
/// @brief  rendering pipeline block representing a node that renders a dynamic shadow map
#pragma once

#include "core-Renderer/RPSoftShadowsNode.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a node that renders a dynamic shadow map
 */
class RPBSoftShadows : public TPipelineBlock< RPSoftShadowsNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBSoftShadows() : TPipelineBlock< RPSoftShadowsNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBSoftShadows( RPSoftShadowsNode& node );

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
