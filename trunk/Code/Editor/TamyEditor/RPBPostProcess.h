/// @file   TamyEditor/RPBPostProcess.h
/// @brief  rendering pipeline block representing a single post process stage
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPPostProcessNode;

///////////////////////////////////////////////////////////////////////////////

class RPBPostProcess : public TRenderingPipelineBlock< RPPostProcessNode >
{
   DECLARE_CLASS( RPBPostProcess )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBPostProcess() : TRenderingPipelineBlock< RPPostProcessNode >() {}

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
