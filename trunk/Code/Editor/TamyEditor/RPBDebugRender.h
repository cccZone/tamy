/// @file   TamyEditor/RPBDebugRender.h
/// @brief  rendering pipeline block representing a node that renders a debug scenes
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPDebugRendererNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a node that renders scenes.
 */
class RPBDebugRender : public TRenderingPipelineBlock< RPDebugRendererNode >
{
   DECLARE_CLASS( RPBDebugRender )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBDebugRender() : TRenderingPipelineBlock< RPDebugRendererNode >() {}

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
