/// @file   TamyEditor/RPBFloat.h
/// @brief  rendering pipeline block representing a float value provider node
#pragma once

#include "core-Renderer/RPFloatNode.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a float value provider node.
 */
class RPBFloat : public TPipelineBlock< RPFloatNode, RenderingPipelineNode >
{
   DECLARE_CLASS( RPBFloat )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBFloat() : TPipelineBlock< RPFloatNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBFloat( RPFloatNode& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_ROUNDED; }
   virtual QColor getBgColor() const { return QColor( 141, 136, 167 ); }
};

///////////////////////////////////////////////////////////////////////////////
