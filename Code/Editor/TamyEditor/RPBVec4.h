/// @file   TamyEditor/RPBVec4.h
/// @brief  rendering pipeline block representing a vector provider node
#pragma once

#include "core-Renderer/RPVec4Node.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a vector provider node.
 */
class RPBVec4 : public TPipelineBlock< RPVec4Node, RenderingPipelineNode >
{
   DECLARE_CLASS( RPBVec4 )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBVec4() : TPipelineBlock< RPVec4Node, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBVec4( RPVec4Node& node );

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
