/// @file   TamyEditor/GBTimer.h
/// @brief  geometry shader pipeline block representing a time provider node
#pragma once

#include "core-Renderer/GNTimer.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * GeometryShader pipeline block representing a time provider node.
 */
class GBTimer : public TPipelineBlock< GNTimer, GeometryShaderNode >
{
   DECLARE_CLASS( GBTimer )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBTimer() : TPipelineBlock< GNTimer, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBTimer( GNTimer& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_ROUNDED; }
   virtual QColor getBgColor() const { return QColor( 151, 180, 167 ); }
};

///////////////////////////////////////////////////////////////////////////////
