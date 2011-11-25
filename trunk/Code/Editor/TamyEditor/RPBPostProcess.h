/// @file   TamyEditor/RPBPostProcess.h
/// @brief  rendering pipeline block representing a single post process stage
#pragma once

#include "core-Renderer/RPPostProcessNode.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPBPostProcess : public TPipelineBlock< RPPostProcessNode, RenderingPipelineNode >
{
   DECLARE_CLASS( RPBPostProcess )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBPostProcess() : TPipelineBlock< RPPostProcessNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBPostProcess( RPPostProcessNode& node );

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
