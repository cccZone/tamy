/// @file   TamyEditor/MBTimer.h
/// @brief  material pipeline block representing a time provider node
#pragma once

#include "core-Renderer/MNTimer.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing a time provider node.
 */
class MBTimer : public TPipelineBlock< MNTimer, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBTimer() : TPipelineBlock< MNTimer, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBTimer( MNTimer& node );

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
