/// @file   TamyEditor/RPBStart.h
/// @brief  rendering pipeline block representing a rendering process start node
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPStartNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a rendering process start node.
 */
class RPBStart : public TRenderingPipelineBlock< RPStartNode >
{
   DECLARE_CLASS( RPBStart )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBStart() : TRenderingPipelineBlock< RPStartNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBStart( RPStartNode& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------

   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }
   virtual QColor getBgColor() const { return QColor( 224, 207, 166 ); }
};

///////////////////////////////////////////////////////////////////////////////
