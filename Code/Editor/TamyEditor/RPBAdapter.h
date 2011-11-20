/// @file   TamyEditor/RPBStart.h
/// @brief  rendering pipeline block representing a rendering process adapter node
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPAdapterNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a rendering process start node.
 */
class RPBAdapter : public TRenderingPipelineBlock< RPAdapterNode >
{
   DECLARE_CLASS( RPBAdapter )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBAdapter() : TRenderingPipelineBlock< RPAdapterNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBAdapter( RPAdapterNode& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_CIRCLE; }
   virtual QColor getBgColor() const { return QColor( 183, 145, 75 ); }
};

///////////////////////////////////////////////////////////////////////////////
