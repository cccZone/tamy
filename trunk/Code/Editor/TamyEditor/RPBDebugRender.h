/// @file   TamyEditor/RPBDebugRender.h
/// @brief  rendering pipeline block representing a node that renders a debug scenes
#pragma once

#include "core-Renderer/RPDebugRendererNode.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a node that renders scenes.
 */
class RPBDebugRender : public TPipelineBlock< RPDebugRendererNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBDebugRender() : TPipelineBlock< RPDebugRendererNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBDebugRender( RPDebugRendererNode& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }
   virtual QColor getBgColor() const { return QColor( 166, 198, 224 ); }
};

///////////////////////////////////////////////////////////////////////////////
