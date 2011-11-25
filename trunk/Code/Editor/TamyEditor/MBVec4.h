/// @file   TamyEditor/MBVec4.h
/// @brief  material pipeline block representing a vector provider node
#pragma once

#include "core-Renderer/MNVec4.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class MNVec4;

///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing a vector provider node.
 */
class MBVec4 : public TPipelineBlock< MNVec4, MaterialNode >
{
   DECLARE_CLASS( MBVec4 )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBVec4() : TPipelineBlock< MNVec4, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBVec4( MNVec4& node );

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
